import rpyc
import time
import threading
import Sofa
import importlib.util
import pathlib
import numpy as np
from multiprocessing import shared_memory
import multiprocessing
import operator


class SOFAClient():
    """
    Client class that connects to a SOFAService running on a local or remote machine
    via RPyC. This allows:
      - Remote launching of a SOFA simulation
      - Access to scene graph objects through RPyC proxying
      - Optional shared memory mechanism for large numpy arrays (faster than sockets)

    This class is typically used on the client side to start the server, connect,
    load scenes, and step the simulation remotely.
    """

    class SOFASharedMemoryProxy():
        """
        Internal proxy object used when shared memory is enabled.

        Instead of fetching large numpy arrays through RPyC sockets (slow, serialized),
        this wrapper ensures that values are accessed directly via memory mapping
        (zero-copy). It dynamically resolves attribute access along the SOFA graph path,
        and switches between RPyC calls and shared memory reads depending on context.

        Not intended for direct use by the end user.
        """

        def __init__(self, client, server, path = ""):
            self.client = client
            self.server = server
            self.path = path

        def __call__(self, *args, **kwds):
            """
            Called when a method of a SOFA object is invoked (e.g. mstate.position.setValue()).

            - Special cases:
                - getValue: returns shared memory-backed numpy array if tracked
                - setValue: writes into shared memory if tracked
            - Otherwise: forwards the call through RPyC.
            """
            
            caller =  operator.attrgetter(self.path)

            lastObject = self.path.split('.')[-1]

            # Special handling for SOFA Data (getValue/setValue)
            # Note: once SofaPython3 bindings provide proper getter/setter,
            # this branch can be simplified.
            if lastObject == "getValue":
                if self.path in self.server.sharedPaths:
                    return self.__internal_wrap_shared_data(self.path)
                else:
                    caller =  operator.attrgetter(self.path)                                          
                    return caller(self.server.exposed_sofa_root).value
                
            elif lastObject == "setValue":
                if self.path in self.server.sharedPaths:
                    self.server.set_data_from_shared(self.path, *args, **kwds)
                else:
                    self.server.set_data_default(self.path, *args, **kwds)


            return caller(self.server.exposed_sofa_root).__call__(*args,**kwds)
        

        def __getattr__(self,item):
            """
            Attribute access redirection:

            - If the attribute corresponds to a shared numpy array `.value`,
              it returns a numpy array backed by shared memory.
            - Otherwise, it either:
                - Navigates deeper into the SOFA graph path
                - Or returns a standard RPyC proxy if not tracked
            """

            # Check if we are still navigating along a tracked shared memory path
            tracked_data = False
            for path in self.server.sharedPaths:
                if self.path in path:
                    tracked_data = True
                    break

            # Special case: accessing `.value` of a shared data
            if tracked_data and self.path in self.server.sharedPaths and item == "value":
                self.server.copy_shared_data_into_memory(self.path)
                # Lazily attach to shared memory only once
                if not self.path in self.client.sharedMemory :
                    self.client.sharedMemory[self.path] = shared_memory.SharedMemory(name=self.server.sharedMemory[self.path].getSharedName())
                # Now create a NumPy array backed by shared memory
                return np.ndarray(self.server.sharedMemory[self.path].shape,str(self.server.sharedMemory[self.path].dtype), buffer = self.client.sharedMemory[self.path].buf)
            

            # If path is no longer tracked, or attribute is not `.value`
            elif not tracked_data or (self.path in self.server.sharedPaths and item != "value"):
                outPath = self.path
                if(outPath != ""):
                    outPath += '.'
                outPath += item   
            
                caller =  operator.attrgetter(outPath)                                          
                return caller(self.server.exposed_sofa_root)
           
            # Still in tracked path, but not yet `.value` → recurse deeper
            outPath = self.path
            if(outPath != ""):
                outPath += '.'
            outPath += item    
            return SOFAClient.SOFASharedMemoryProxy(self.client, self.server, outPath)
        
    def __init__(self):
        self.sharedMemory = {}
        
    def start_server(self, hostname="localhost", port=18813):
        """
        Start a SOFAService server in a separate process.
        The service will expose the SOFA simulation API through RPyC.
        """
        self.server = rpyc.ForkingServer(service=SOFAService(), hostname=hostname, port=port,protocol_config={'allow_public_attrs': True, 'allow_all_attrs': True,'allow_pickle': True })

        self.serverProcess = multiprocessing.Process(target = self.server.start)
        self.serverProcess.start()

    def connect_client(self,hostename="localhost", port=18813,  number_of_attempt = 10, wait_time = 0.1):
        """
        Connect the client to a running SOFAService server.

        Args:
            hostename: target host
            port: RPyC port (default: 18813)
            number_of_attempt: how many retries before failing
            wait_time: sleep time between retries
        """
        it = 0
        connected = self.__internal_connect_to_client(hostename=hostename, port=port)
        
        while(not connected and it < number_of_attempt):
            time.sleep(wait_time)
            connected = self.__internal_connect_to_client(hostename=hostename, port=port)
            it += 1

        if(connected):
            # Wrap step simulation in async executor (non-blocking call)
            self.async_step_executor = rpyc.async_(self.connection.root.step_simulation)

        return connected


    def __internal_connect_to_client(self, hostename, port):
        """
        Attempt to open a socket to the server. Returns True if successful, False otherwise.
        """
        try:
            self.connection = rpyc.connect(hostename, port, config = {'allow_public_attrs': True, "allow_all_attrs": True, "allow_pickle": True})
        except ConnectionRefusedError:
            return False
        return True
    
    def load_scene(self, inputValue):
        """
        Load a SOFA scene on the server.

        - If inputValue is a string: interpreted as an absolute path on the server to a Python file
          containing a `createScene(root)` function.
        - If inputValue is a method: directly passed and executed on the server.

        Examples:
            load_scene("/path/to/ExampleFileOnRemote.py")
            load_scene(my_createScene_function)
        """
        if(isinstance(inputValue, str )):
            self.connection.root.exposed_build_scene_graph_from_file(inputValue)
        else:
            self.connection.root.exposed_build_scene_graph_from_method(inputValue)

    def asynch_step(self):
        """
        Launch a single simulation step asynchronously.
        Returns a promise-like object to check completion.
        """
        return self.async_step_executor()

    def stop_server(self):
        """
        Kill the server process and close the client connection.
        """
        self.connection.close()
        self.serverProcess.terminate()

    def __getattr__(self, item):
        """
        Convenience proxy:
        Allows direct access to objects in the SOFA root as if they were local.

        Example:
            client.sofa_root.liver.mstate.position.value
        """
        if item == "sofa_root" and self.connection.root.sharedMemoryIsSet and self.connection.root.sharedMemoryIsUsed :
            return SOFAClient.SOFASharedMemoryProxy(client = self, server = self.connection.root)
        return(getattr(self.connection.root,item))
        

class SOFAService(rpyc.SlaveService):
    """
    RPyC service exposing a SOFA simulation.

    This service:
      - Manages the SOFA scene graph
      - Executes simulation steps
      - Optionally sets up shared memory for numpy array Data fields
      - Supports threaded continuous execution (play/pause/reset)

    Intended to be run in a separate process, typically started by SOFAClient.
    """

    class SharedMemoryInfo():
        """
        Container for metadata about one shared memory buffer.

        Stores numpy dtype/shape information alongside the SharedMemory object,
        allowing clients to attach and reconstruct numpy arrays correctly.
        """
        def __init__(self,shape, dtype, sharedMem):
            self.shape = shape
            self.dtype = dtype
            self.sharedMem = sharedMem
            
        def getSharedName(self):
            return self.sharedMem.name

        def getDataInfo(self):
            return {"shape" : self.shape, "dtype" : self.dtype}


    exposed_sofa_root : Sofa.Core.Node
    sharedPaths : dict
    
    def __init__(self, *args, **kwargs):
        rpyc.Service.__init__(self,*args, **kwargs)
        self.animationThread = None
        self.sharedMemory = None
        self.sharedMemoryIsSet = False
        self.sharedMemoryIsUsed = False


    def on_connect(self, conn):
        """
        Called when a client connects.
        Initializes an empty root node so the graph can be immediately modified remotely.
        """
        self.exposed_sofa_root = Sofa.Core.Node("root")
        self.animate = False
        pass

    def on_disconnect(self, conn):
        """
        Called when a client disconnects.
        Ensures simulation is paused and shared memory is properly released.
        """
        self.exposed_pause_simulation()
        for sm in self.sharedMemory:
            sm.sharedMem.unlink()
            sm.sharedMem.close()

    def exposed_build_scene_graph_from_method(self, createScene):
        """
        Rebuild scene graph from a function received via RPyC.
        The function must accept a root node and build the SOFA graph.
        """
        self.exposed_sofa_root = Sofa.Core.Node("root")
        createScene(self.exposed_sofa_root)
        self.exposed_init_root()

    def exposed_build_scene_graph_from_file(self, filename:str):
        """
        Rebuild scene graph from a Python file local to the server.
        The file must define a `createScene(root)` function.
        """

        moduleName = pathlib.Path(filename).name.split('.')[0]
        spec=importlib.util.spec_from_file_location(moduleName,filename)
        foo = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(foo)

        self.exposed_sofa_root = Sofa.Core.Node("root")
        foo.createScene(self.exposed_sofa_root)
        self.exposed_init_root()


    def exposed_setup_shared_memory_for_data(self, dataPaths:list[str], delayed=False):
        """
        Setup shared memory buffers for specific data paths in the scene graph.

        Args:
            dataPaths: list of data paths (e.g. "liver.mstate.position")
            delayed: if True, defer setup (useful if data size is only finalized after one timestep)
        """
        self.paths_for_shared_mem = dataPaths
        if(not delayed):
            self.paths_for_shared_mem = self.__internal_setup_shared_memory()


    def __internal_setup_shared_memory(self):
        """
        Internal: create shared memory buffers for all tracked paths.

        Only numpy arrays are supported. Others are skipped.
        """
        self.sharedMemory = {}
        self.sharedPaths = []
        self.sharedMemoryIsSet = True
        for paths in self.paths_for_shared_mem:

            paths = paths.replace('/','.')
            while len(paths) > 1 and paths[0] in "@." :
                paths = paths[1:]

            caller =  operator.attrgetter(paths)                                          
            data = caller(self.exposed_sofa_root).value
            if( isinstance(data, np.ndarray)):
                self.sharedMemory[paths] = SOFAService.SharedMemoryInfo(data.shape, data.dtype, shared_memory.SharedMemory(create=True, size=data.nbytes))
                self.sharedPaths.append(paths)
                print(f"Sharing {data.nbytes} bytes for data {paths}.")

            else:
                print(f"Not creating a shared memory for data {paths} because it is no a numpy array")
        self.sharedMemoryIsUsed = len(self.sharedPaths) != 0
        return self.sharedPaths
    

    def exposed_copy_shared_data_into_memory(self,shared_name):
        """
        Copy latest value of given data path into shared memory buffer.
        Client can then access it directly via numpy array mapping.
        """
        shm = shared_memory.SharedMemory(name=self.sharedMemory[shared_name].getSharedName())
        caller =  operator.attrgetter(shared_name)                                          
        data = caller(self.exposed_sofa_root).value
        b = np.ndarray(data.shape, dtype=data.dtype, buffer=shm.buf)
        b[:] = data[:]  


    def getSharedMemoryNames(self):
        """Return list of all data paths currently shared via shared memory."""
        return self.sharedPaths

    def exposed_init_root(self):
        """Initialize the root node."""
        if(not self.exposed_sofa_root.isInitialized()):
            Sofa.Simulation.initRoot(self.exposed_sofa_root)

    def exposed_step_simulation(self):
        """
        Run one step of the simulation.
        If shared memory hasn’t been set up yet, attempt setup now.
        """

        Sofa.Simulation.animate(self.exposed_sofa_root, self.exposed_sofa_root.dt.value)

        if(not self.sharedMemoryIsSet):
            self.__internal_setup_shared_memory()



    #################################################
    ### Multithreaded automatic execution methods
    ###
    ### WARNING: Use with caution. Known to be unstable in some contexts,
    ### but provided as experimental support for continuous stepping.
    #################################################


    def __wait_for_the_animation_to_stop(self):
        """Wait for the simulation thread to stop (blocking join)."""
        if(self.animationThread is not None and self.animationThread.is_alive()):
            self.animationThread.join()


    def __simulation_loop(self):
        """Continuous simulation loop run by background thread."""

        while self.animate:
            Sofa.Simulation.animate(self.exposed_sofa_root, self.exposed_sofa_root.dt.value)
    

    def exposed_start_simulation(self):
        """
        Start continuous simulation loop in a separate thread.
        If already running, does nothing.
        """
        if(self.animate):
            return
        
        self.__wait_for_the_animation_to_stop()

        self.animate=True
        self.simulationThread = threading.Thread(target = self.__simulation_loop)
        self.simulationThread.start()


    def exposed_pause_simulation(self):
        """
        Pause simulation (stop stepping in background).
        Can be resumed later by calling exposed_start_simulation().
        """
        self.animate = False
        self.__wait_for_the_animation_to_stop()
        

    def exposed_reset_simulation(self):
        """
        Reset simulation to t=0.
        If it was running, restart loop afterwards.
        """
        need_restart = False
        if(self.animate):
            need_restart = True

        self.animate = False
        self.__wait_for_the_animation_to_stop()

        Sofa.Simulation.reset()
        
        if(need_restart):
            self.exposed_start_simulation()


        
        

import multiprocessing
import threading
import time
import rpyc
from SOFAService import SOFAService, SOFAClient
import Sofa


if __name__ == "__main__":


    SC = SOFAClient()
    SC.start_server(port=18818)
    SC.connect_client(port=18818)

    SC.load_scene("../liver.py")

    SC.setup_shared_memory_for_data(["Liver/dofs.position","Liver/Surf/spheres.position"])



    asynch_step = None
    currentTime = 0.0
    while currentTime<0.2:
        if(asynch_step is None or asynch_step.ready):
            # Time to get data from object
            currentTime = SC.sofa_root.getTime()
            print(currentTime)
    
            print(f"This comes with the socket : {SC.sofa_root.Liver.cg_odesolver.name.value}")
            print(f"This comes with shared memory : {SC.sofa_root.Liver.Surf.spheres.position.value}")
            print(f"This getValue comes with the socket : {SC.sofa_root.Liver.cg_odesolver.name.getValue()}")
            print(f"This getValue comes with shared memory : {SC.sofa_root.Liver.Surf.spheres.position.getValue()}")
            SC.sofa_root.Liver.cg_odesolver.printLog.setValue(True)
            SC.sofa_root.Liver.Surf.spheres.position.setValue([[0,0,0]])

            # Launch next step
            asynch_step = SC.asynch_step()
        else:
            print("waiting 0.1 sec")
            time.sleep(0.1)


    # SC.pause_simulation()
    SC.stop_server()
    

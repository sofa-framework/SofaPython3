import meshio
import gmsh
import pygmsh
import Sofa

resolution = 0.01
# Channel parameters
L = 2.2
H = 0.41
c = [0.2, 0.2, 0]
r = 0.05


def generateMesh():
    # Initialize empty geometry using the build in kernel in GMSH
    geometry = pygmsh.geo.Geometry()
    # Fetch model we would like to add data to
    model = geometry.__enter__()
    # Add circle
    circle = model.add_circle(c, r, mesh_size=resolution)
    # Add points with finer resolution on left side
    points = [
        model.add_point((0, 0, 0), mesh_size=resolution),
        model.add_point((L, 0, 0), mesh_size=5 * resolution),
        model.add_point((L, H, 0), mesh_size=5 * resolution),
        model.add_point((0, H, 0), mesh_size=resolution),
    ]

    # Add lines between all points creating the rectangle
    channel_lines = [
        model.add_line(points[i], points[i + 1]) for i in range(-1, len(points) - 1)
    ]

    # Create a line loop and plane surface for meshing
    channel_loop = model.add_curve_loop(channel_lines)
    plane_surface = model.add_plane_surface(channel_loop, holes=[circle.curve_loop])

    # Call gmsh kernel before add physical entities
    model.synchronize()
    volume_marker = 6
    model.add_physical([plane_surface], "Volume")
    model.add_physical([channel_lines[0]], "Inflow")
    model.add_physical([channel_lines[2]], "Outflow")
    model.add_physical([channel_lines[1], channel_lines[3]], "Walls")
    model.add_physical(circle.curve_loop.curves, "Obstacle")
    geometry.generate_mesh(dim=3)
    gmsh.write("mesh.msh")
    gmsh.clear()
    geometry.__exit__()

    
def createScene(root):

    root.addObject("RequiredPlugin", pluginName=["Sofa.Component.IO.Mesh",
    "Sofa.GL.Component.Rendering3D", "Sofa.Component.Topology.Container.Dynamic","Sofa.Component.StateContainer"])

    root.addObject("DefaultAnimationLoop")
    loader = root.addObject("MeshGmshLoader", name="GmshLoader", filename="mesh.msh")
    root.addObject("TriangleSetTopologyContainer", name="Topology", src="@GmshLoader")
    root.addObject("TriangleSetGeometryAlgorithms", name="Geometry", drawTriangles=True)
    root.addObject("MechanicalObject", name="DoFsContainer")

    return root




def main():
    import SofaImGui
    import Sofa.Gui

    generateMesh()

    # Create the root node
    root = Sofa.Core.Node("root")
    # Call the below 'createScene' function to create the scene graph
    createScene(root)
    # Initialize the graph from the root node
    Sofa.Simulation.initRoot(root)

    # Find out the supported GUIs
    print ("Supported GUIs are: " + Sofa.Gui.GUIManager.ListSupportedGUI(","))
    # Launch the GUI (qt or qglviewer)
    Sofa.Gui.GUIManager.Init("myscene", "imgui")
    Sofa.Gui.GUIManager.createGUI(root, __file__)
    Sofa.Gui.GUIManager.SetDimension(1080, 1080)
    # Initialization of the scene will be done here
    Sofa.Gui.GUIManager.MainLoop(root)
    Sofa.Gui.GUIManager.closeGUI()
    print("GUI was closed")
    print("Simulation is done.")


# Function used only if this script is called from a python environment
if __name__ == '__main__':
    main()


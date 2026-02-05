from CGAL.CGAL_Polyhedron_3 import Polyhedron_3
from CGAL.CGAL_Mesh_3 import Polyhedral_mesh_domain_3
from CGAL.CGAL_Mesh_3 import Mesh_3_parameters
from CGAL.CGAL_Mesh_3 import Default_mesh_criteria
from CGAL.CGAL_Kernel import Point_3
from CGAL.CGAL_Convex_hull_3 import convex_hull_3, is_strongly_convex_3
from CGAL import CGAL_Mesh_3
from pypcd4 import PointCloud

import sys
import argparse 
import os
import random
import time


from cgal_utils import CGAL_Mesh_from, CGAL_Mesh_3_IO_Util, ReadPolyData, tic, toc
from mesh_from_polyhedron import CGAL_Mesh_from_polyhedron

class CGAL_Mesh_from_pointcloud(CGAL_Mesh_from):
    def __init__(self, pointcloud ):
        print(f"Transformaing input data into CGAL data structure...")
        tic()
        self.pointcloud = []
        for point in pointcloud:
            self.pointcloud.append(Point_3(point[0],point[1],point[2]))
        print(f"Done ! Took {toc()}")


    def generate(self, criteria , refiner = CGAL_Mesh_from.Refiner_input(refiner_type=CGAL_Mesh_from.Refiner.NONE)):
        print(f"Generating convex hull...")
        tic()

        self.polyhedron = Polyhedron_3()
        convex_hull_3(self.pointcloud, self.polyhedron)



        print(f"Done ! Took {toc()}")
        print(f"Convex hull has {self.polyhedron.size_of_vertices()} vertices and is strongly convex: {is_strongly_convex_3(self.polyhedron)}")

        cmfp = CGAL_Mesh_from_polyhedron(polyhedron=self.polyhedron)
        cmfp.generate(criteria, refiner)
        self.IOUtil = cmfp.IOUtil

    def write_out(self, filename):
        self.IOUtil.write_out(filename)

# Function called when the scene graph is being created
def createScene(root, file_name):

    root.addObject("RequiredPlugin", pluginName=["Sofa.Component.IO.Mesh",
    "Sofa.GL.Component.Rendering3D", "Sofa.Component.Topology.Container.Dynamic","Sofa.Component.StateContainer"])

    root.addObject("DefaultAnimationLoop")
    loader = root.addObject("MeshVTKLoader", name="VTKLoader", filename=file_name)
    root.addObject("TetrahedronSetTopologyContainer", name="Topology", src="@VTKLoader")
    root.addObject("TetrahedronSetGeometryAlgorithms", name="Geometry", drawTetrahedra=True)
    root.addObject("MechanicalObject", name="DoFsContainer")

    return root

if __name__ == "__main__":    

    parser = argparse.ArgumentParser(description="Creates a mesh from a point cloud using convex hull.")   

    parser.add_argument("-i", "--input", default='data/point_cloud.pcd', help="The input file containing the surface. Format must be taken form ['.pcd']")
    parser.add_argument( "-o", "--output", default='pointCloudMesh.vtk', help="The output file to save the computed volumetric mesh")
    parser.add_argument( "-r", "--refiner", default='None', help="Use refiner to erfine the mesh. Values are amongst ['None', 'Lloyd', 'Odt', 'Perturb']")
    parser.add_argument( "-c", "--criteria", default='"facet_angle=25 edge_size=0.4 facet_size=0.15 facet_distance=0.008 cell_radius_edge_ration=3"', help="Set of parameters in the form of \"facet_angle=25 edge_size=0.4 facet_size=0.15 facet_distance=0.008 cell_radius_edge_ration=3\" that can be customized. If one is not specified, its default value is used")
    parser.add_argument( "-d", "--display", action='store_true', help="If flag is added, a SOFA scene will be launched showing the generated mesh")

    args = parser.parse_args() 


    tic(1)

    pc = PointCloud.from_path(args.input)
    PC = pc.numpy(("x", "y", "z"))

    cmfp = CGAL_Mesh_from_pointcloud(PC.tolist())


    #Criterial
    class criteriaContainer:
        def __init__(self):
            self.facet_angle=25
            self.edge_size=0.15
            self.facet_size=0.15
            self.facet_distance=0.008
            self.cell_radius_edge_ratio=3

    selectedCriterias = criteriaContainer()

    for passedCriteria in args.criteria.split(' '):
        if len(passedCriteria.split('=')) >1:
            if passedCriteria.split('=')[0] in selectedCriterias.__dict__:
                setattr(selectedCriterias, passedCriteria.split('=')[0], float(passedCriteria.split('=')[1]) )

    criteria = Default_mesh_criteria()
    criteria.facet_angle(selectedCriterias.facet_angle).edge_size(selectedCriterias.edge_size).facet_size(selectedCriterias.facet_size).facet_distance(selectedCriterias.facet_distance).cell_radius_edge_ratio(selectedCriterias.cell_radius_edge_ratio)

    #Refiner
    match args.refiner:
        case 'Lloyd':
            refinerName = 'Lloyd'
            refiner =  CGAL_Mesh_from.Refiner_input(refiner_type=CGAL_Mesh_from.Refiner.LLOYD)
        case 'Odt':
            refinerName = 'Odt'
            refiner =  CGAL_Mesh_from.Refiner_input(refiner_type=CGAL_Mesh_from.Refiner.ODT)
        case 'Perturb':
            refinerName = 'Perturb'
            refiner =  CGAL_Mesh_from.Refiner_input(refiner_type=CGAL_Mesh_from.Refiner.PERTURB)
        case _:
            refinerName = 'None'
            refiner =  CGAL_Mesh_from.Refiner_input(refiner_type=CGAL_Mesh_from.Refiner.NONE)


    print(f"Launching mesh generation with following parameter : ")
    print(f" - Criteria : facet_angle = {selectedCriterias.facet_angle}, edge_size = {selectedCriterias.edge_size}, facet_size = {selectedCriterias.facet_size}, facet_distance = {selectedCriterias.facet_distance}, cell_radius_edge_ratio = {selectedCriterias.cell_radius_edge_ratio}")
    print(f" - Refiner : {refinerName}")
    cmfp.generate(criteria, refiner)

    cmfp.write_out(args.output)
    print(f"The script took a total of {toc(1)}")

    if args.display:

        import Sofa
        import SofaImGui
        import Sofa.Gui

        #Create the root node
        root = Sofa.Core.Node("root")
        # Call the below 'createScene' function to create the scene graph
        createScene(root, args.output)
        Sofa.Simulation.initRoot(root)

        # Launch the GUI (imgui is now by default, to use Qt please refer to the example "basic-useQtGui.py")
        Sofa.Gui.GUIManager.Init("myscene", "imgui")
        Sofa.Gui.GUIManager.createGUI(root, __file__)
        Sofa.Gui.GUIManager.SetDimension(1080, 1080)
        # Initialization of the scene will be done here
        Sofa.Gui.GUIManager.MainLoop(root)
        Sofa.Gui.GUIManager.closeGUI()






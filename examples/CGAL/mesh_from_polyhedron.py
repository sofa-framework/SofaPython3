from CGAL.CGAL_Polyhedron_3 import Polyhedron_3, Polyhedron_modifier
from CGAL.CGAL_Mesh_3 import Mesh_3_Complex_3_in_triangulation_3
from CGAL.CGAL_Mesh_3 import Polyhedral_mesh_domain_3
from CGAL.CGAL_Mesh_3 import Mesh_3_parameters
from CGAL.CGAL_Mesh_3 import Default_mesh_criteria
from CGAL.CGAL_Kernel import Point_3
from CGAL import CGAL_Mesh_3

import sys
import argparse 
import os


from cgal_utils import CGAL_Mesh_from, CGAL_Mesh_3_IO_Util, ReadPolyData, tic, toc

class CGAL_Mesh_from_polyhedron(CGAL_Mesh_from):
    def __init__(self, filename = None, polyhedron = None):
        if(filename is None and polyhedron is None):
            raise ValueError("Need either a filename to load or a CGAl polyhedron already build")
        if(filename is not None):
            print(f"Loading polyhedron from {filename}...")
            tic()
            
            mesh = ReadPolyData(filename)
            self.polyhedron = Polyhedron_3()
            pm =  Polyhedron_modifier()

            pm.begin_surface(3,1)
            for i in range(mesh.GetNumberOfPoints()):
                pt = mesh.GetPoint(i)
                pm.add_vertex(Point_3(pt[0],pt[1],pt[2]))


            for i in range(mesh.GetNumberOfCells()):
                pm.begin_facet()
                for j in range(mesh.GetCell(i).GetNumberOfPoints()):
                    pm.add_vertex_to_facet(int(mesh.GetCell(i).GetPointId(j)))
                pm.end_facet()


            self.polyhedron.delegate(pm) 
            print(f"Done ! Took {toc()}")
        else:
            self.polyhedron = polyhedron
        
        print(f"Polyhedron info from input (vertices, facets, edges) = {(self.polyhedron.size_of_vertices(), self.polyhedron.size_of_facets(), self.polyhedron.size_of_halfedges()/2)}")


    def generate(self, criteria , refiner = CGAL_Mesh_from.Refiner_input(refiner_type=CGAL_Mesh_from.Refiner.NONE)):
        print(f"Generating mesh...")
        tic()

        # Create domain
        domain = Polyhedral_mesh_domain_3(self.polyhedron)
        params = Mesh_3_parameters()
        params.no_exude()
        params.no_perturb()
        match refiner.refiner_type:
            case CGAL_Mesh_from.Refiner.LLOYD:
                params.set_lloyd(refiner.time_limit, refiner.max_iteration_number, refiner.convergence, refiner.free_bound)
            case CGAL_Mesh_from.Refiner.ODT:
                params.set_odt(refiner.time_limit, refiner.max_iteration_number, refiner.convergence, refiner.free_bound)
            case CGAL_Mesh_from.Refiner.PERTURB:
                params.set_perturb(refiner.time_limit, refiner.silver_bound)


        # Mesh generation
        c3t3 = CGAL_Mesh_3.make_mesh_3(domain, criteria, params)
        print(f"Done ! Took {toc()}")

        self.IOUtil = CGAL_Mesh_3_IO_Util(c3t3)
        self.IOUtil.extract([CGAL_Mesh_3_IO_Util.Elem.POINTS, CGAL_Mesh_3_IO_Util.Elem.TETRA])

    def write_out(self, filename):
        self.IOUtil.write_out(filename)
        

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Creates a mesh from an input file containing a polyhedron representing the surface.")   

    parser.add_argument("-i", "--input", default='data/torus.obj', help="The input file containing the surface. Format must be taken form ['.g', '.obj', '.stl', '.ply', '.vtk', '.vtp']")
    parser.add_argument( "-o", "--output", default='torusVol.vtk', help="The output file to save the computed volumetric mesh")
    parser.add_argument( "-r", "--refiner", default='None', help="Use refiner to erfine the mesh. Values are amongst ['None', 'Lloyd', 'Odt', 'Perturb']")  
    parser.add_argument( "-c", "--criteria", default='"facet_angle=25 edge_size=0.4 facet_size=0.15 facet_distance=0.008 cell_radius_edge_ration=3"', help="Set of parameters in the form of \"facet_angle=25 edge_size=0.4 facet_size=0.15 facet_distance=0.008 cell_radius_edge_ration=3\" that can be customized. If one is not specified, its default value is used")  
    args = parser.parse_args() 

    tic(1)
    cmfp = CGAL_Mesh_from_polyhedron(filename=args.input)

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




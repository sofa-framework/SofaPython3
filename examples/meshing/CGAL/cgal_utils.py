from CGAL.CGAL_Polyhedron_3 import Polyhedron_3, Polyhedron_modifier
from CGAL.CGAL_Mesh_3 import Mesh_3_Complex_3_in_triangulation_3
from CGAL.CGAL_Mesh_3 import Polyhedral_mesh_domain_3
from CGAL.CGAL_Mesh_3 import Mesh_3_parameters
from CGAL.CGAL_Mesh_3 import Default_mesh_criteria
from CGAL.CGAL_Kernel import Point_3
from CGAL import CGAL_Mesh_3

import numpy as np
import dataclasses
from typing import List, Optional

from enum import Enum
from pathlib import Path

import time

from vtkmodules.vtkIOGeometry import (
     vtkBYUReader,
     vtkOBJReader,
     vtkSTLReader
 )
from vtkmodules.vtkIOLegacy import vtkPolyDataReader
from vtkmodules.vtkIOPLY import vtkPLYReader
from vtkmodules.vtkIOXML import vtkXMLPolyDataReader

import vtk
from vtk.util import numpy_support
from vtk.numpy_interface import algorithms as algs

timestamp = {}

def tic(hash = 0):
    global timestamp
    timestamp[hash] = time.time_ns()

def toc(hash = 0):
    global timestamp
    return f"{((time.time_ns() - timestamp[hash])/1000000):.3f} miliseconds"
    

def ReadPolyData(file_name):
    #FROM Vtk examples https://examples.vtk.org/site/Python/Snippets/ReadPolyData/

    valid_suffixes = ['.g', '.obj', '.stl', '.ply', '.vtk', '.vtp']

    path = Path(file_name)
    if path.suffix:
        ext = path.suffix.lower()
    if path.suffix not in valid_suffixes:
        print(f'No reader for this file suffix: {ext}')
        return None
    else:
        if ext == ".ply":
            reader = vtkPLYReader()
            reader.SetFileName(file_name)
            reader.Update()
            poly_data = reader.GetOutput()
        elif ext == ".vtp":
            reader = vtkXMLPolyDataReader()
            reader.SetFileName(file_name)
            reader.Update()
            poly_data = reader.GetOutput()
        elif ext == ".obj":
            reader = vtkOBJReader()
            reader.SetFileName(file_name)
            reader.Update()
            poly_data = reader.GetOutput()
        elif ext == ".stl":
            reader = vtkSTLReader()
            reader.SetFileName(file_name)
            reader.Update()
            poly_data = reader.GetOutput()
        elif ext == ".vtk":
            reader = vtkPolyDataReader()
            reader.SetFileName(file_name)
            reader.Update()
            poly_data = reader.GetOutput()
        elif ext == ".g":
            reader = vtkBYUReader()
            reader.SetGeometryFileName(file_name)
            reader.Update()
            poly_data = reader.GetOutput()

        return poly_data


class CGAL_Mesh_3_IO_Util(object):
    
    points    : np.array
    triangles : np.array
    tetras    : np.array
    
    class Elem(Enum):
        POINTS = 1
        TRIANGLES = 3
        TETRA = 4

    def __init__(self,mesh):
        self.mesh = mesh

    def extract(self, elems : list[Elem]):

        print(f"Extracting data into numpy objects...")
        tic()

        for elem in elems:
            vnbe = {}
            match elem:
                case CGAL_Mesh_3_IO_Util.Elem.TRIANGLES:
                    for elemf in self.mesh.facets():
                        for i in range(3):
                            if not elemf.vertex in vnbe:
                                vnbe[elemf.vertex(i)] = 1 
                            else:
                                vnbe[elem.vertex(i)] += 1 
                case CGAL_Mesh_3_IO_Util.Elem.TETRA:
                    for elemc in self.mesh.cells():
                        for i in range(4):
                            if not elemc.vertex in vnbe:
                                vnbe[elemc.vertex(i)] = 1 
                            else:
                                vnbe[elemc.vertex(i)] += 1 

            
            V = {}
            it = 0
            for vertice in self.mesh.triangulation().finite_vertices():
                if vertice in vnbe:
                    V[vertice] = it
                    it += 1 

            if CGAL_Mesh_3_IO_Util.Elem.POINTS in elems:
                self.points = np.empty((len(V),3))
                id = 0
                for key in V.keys():
                    self.points[id][:] = [self.mesh.triangulation().point(key).x(), self.mesh.triangulation().point(key).y(), self.mesh.triangulation().point(key).z()]
                    id+=1

            match elem:
                case CGAL_Mesh_3_IO_Util.Elem.TRIANGLES:
                    self.triangles = np.empty((self.mesh.number_of_facets(),3), dtype=np.int32)
                    id = 0
                    for elemt in self.mesh.facets():
                        self.triangles[id][:]  = [V[elemt.vertex(0)],V[elemt.vertex(1)],V[elemt.vertex(2)]]
                        id+= 1 
                case CGAL_Mesh_3_IO_Util.Elem.TETRA:
                    self.tetras = np.empty((self.mesh.number_of_cells(),4), dtype=np.int32)
                    id = 0
                    for elemc in self.mesh.cells():
                        self.tetras[id][:]  = [V[elemc.vertex(0)],V[elemc.vertex(1)],V[elemc.vertex(2)], V[elemc.vertex(3)]]
                        id += 1
        print(f"Done ! Took {toc()}")
        if CGAL_Mesh_3_IO_Util.Elem.TRIANGLES in elems:
            print(f"Extracted {self.points.shape[0]} points and {self.triangles.shape[0]} triangles")
        if CGAL_Mesh_3_IO_Util.Elem.TETRA in elems:
            print(f"Extracted {self.points.shape[0]} points and {self.tetras.shape[0]} tetras")

            

    def write_out(self, filename):
        path = Path(filename)
        ext = path.suffix.lower()
        if ext != ".vtk" and ext != ".vtu":
            print("Only vtk or vtu extension are suported")
            return
        
        if (not "tetras"  in self.__dict__) and ext == ".vtu":
            print("VTU only supported for tetrahedral meshes. Use vtk instead")
            return

        print(f"Saving into {filename}...")
        tic()

        if "tetras"  in self.__dict__:
            ugrid = vtk.vtkUnstructuredGrid()
        else:
            ugrid = vtk.vtkPolyData()
        
        vtk_points = vtk.vtkPoints()
        vtk_points.SetData(numpy_support.numpy_to_vtk(self.points, deep=True))
        ugrid.SetPoints(vtk_points)


        if "triangles" in self.__dict__:
            for triangle in self.triangles:
                vtkTriangleObj = vtk.vtkTriangle()
                vtkTriangleObj.GetPointIds().SetId(0,triangle[0])
                vtkTriangleObj.GetPointIds().SetId(1,triangle[1])
                vtkTriangleObj.GetPointIds().SetId(2,triangle[2])
                ugrid.InsertNextCell(vtkTriangleObj.GetCellType(), vtkTriangleObj.GetPointIds())
        if "tetras"  in self.__dict__:
            for tetra in self.tetras:
                vtkTetraObj = vtk.vtkTetra()
                vtkTetraObj.GetPointIds().SetId(0,tetra[0])
                vtkTetraObj.GetPointIds().SetId(1,tetra[1])
                vtkTetraObj.GetPointIds().SetId(2,tetra[2])
                vtkTetraObj.GetPointIds().SetId(3,tetra[3])
                ugrid.InsertNextCell(vtkTetraObj.GetCellType(), vtkTetraObj.GetPointIds())
        
        
        if "tetras"  in self.__dict__:
            writer = vtk.vtkUnstructuredGridWriter()
        else:
            writer = vtk.vtkPolyDataWriter()
        writer.SetFileVersion(42)
        writer.SetInputData(ugrid)
        writer.SetFileName(filename)
        writer.Write()
        print(f"Done ! Took {toc()}")





class CGAL_Mesh_from(object):
    
    class Refiner(Enum):
        NONE = 0
        LLOYD = 1
        ODT = 3
        PERTURB = 4

    @dataclasses.dataclass
    class Refiner_input():
        refiner_type : Enum

        time_limit : Optional[float] = 20.0         #(ALL)              to set up, in seconds, a CPU time limit after which the optimization process is stopped. This time is measured using CGAL::Real_timer. 0 means that there is no time limit. 
        max_iteration_number : Optional[int] = 200  #(LLOYD & ODT only) limit on the number of performed iterations. 0 means that there is no limit on the number of performed iterations.   
        convergence : Optional[int] = 0.0           #(LLOYD & ODT only) threshold ratio of stopping criterion based on convergence: the optimization process is stopped when at the last iteration the displacement of any vertex is less than a given fraction of the length of the shortest edge incident to that vertex.
        free_bound : Optional[bool] = False         #(LLOYD & ODT only) designed to reduce running time of each optimization iteration. Any vertex that has a displacement less than a given fraction of the length of its shortest incident edge, is frozen (i.e. is not relocated). The parameter freeze_bound gives the threshold ratio. If it is set to 0, freezing of vertices is disabled. 
        silver_bound : Optional[bool] = False       #(PERTURB only)     is designed to give, in degrees, a targeted lower bound on dihedral angles of mesh cells. The exudation process considers in turn all the mesh cells that have a smallest dihedral angle less than sliver_bound and tries to make them disappear by weighting their vertices. The optimization process stops when every cell in the mesh achieves this quality. The default value is 0 and means that there is no targeted bound: the exuder then runs as long as it can improve the smallest dihedral angles of the set of cells incident to some vertices. 
        

    IOUtil : CGAL_Mesh_3_IO_Util

    def __init__(self):
        pass

    def generate(self):
        pass


    def __getattr__(self, name):
        match name:
            case "points":
                return self.IOUtil.points
            case "triangles":
                return self.IOUtil.triangles
            case "tetras":
                return self.IOUtil.tetras
            case _:
                if name in self.__dict__:
                    return self.__dict__[name]
                else:
                    raise AttributeError()
    



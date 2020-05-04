# -*- coding: utf-8 -*-
# This file requires gmshpy to be installed. 
# Author: stefan.escaida-navarro@inria.fr

"""
.. autosummary::

    meshFromParametricGeometry    

splib.geometric.gmsh.meshFromParametricGeometry
*********************
.. autofunction:: meshFromParametricGeometry

splib.geometric.gmsh.createScene
********************
.. autofunction:: createScene

"""



import locale
import logging

class LocaleManager:    
    def __init__(self, localename):
        self.name = localename
    def __enter__(self):
        self.orig = locale.setlocale(locale.LC_CTYPE)
        locale.setlocale(locale.LC_ALL, self.name)
    def __exit__(self, exc_type, exc_value, traceback):
        locale.setlocale(locale.LC_ALL, self.orig)
 
def meshVolumetricFromSurface(filepath, outputdir='autogen/',
                              Lloyd=False,
                              ODT=False,
                              Perturb=True,
                              Exude=True,
                              Edge_Size=0.0, 
                              Facet_Angle=25, 
                              Facet_Size=15, 
                              Facet_Distance=0.1,
                              Cell_Radius_Edge_Ratio=3.0,
                              Cell_Size=0.0):
        """
        Generate a volumetric mesh from a surface mesh using the python bindings for CGAL
        """ 
        import splib.caching.cacher as cch
        import pygalmesh
        import meshio
        import os
        import numpy as np
        #import locale        
        #locale.setlocale(locale.LC_ALL, "C")
        
        with LocaleManager('C'):       
            # Set options from kwargs     
            
            OptionStrings = ['Lloyd', 'ODT', 'Perturb', 'Exude', 'Edge_Size', 'Facet_Angle', 'Facet_Size', 'Facet_Distance', 'Cell_Radius_Edge_Ratio', 'Cell_Size'] # These are all the possible arguments, we just hash all of their values
            Values = [Lloyd, ODT, Perturb, Exude, Edge_Size, Facet_Angle, Facet_Size, Facet_Distance, Cell_Radius_Edge_Ratio, Cell_Size]
            
            kwargs = dict(zip(OptionStrings,Values)) 
                
            Refresh = False
            OutputFilePath = ''        
            FileNameWithExt = os.path.split(filepath)[1]
            FileNameNoExt = os.path.splitext(FileNameWithExt)[0]                
            
            Refresh, OutputFilePath = cch.cacher(InputFilePath=filepath, OutputDir=outputdir, OutputFileExtension='.vtk', kwargsdict=kwargs, OutputFileName=FileNameNoExt)
        
            if Refresh:
                logging.info(' Beginning volumetric meshing with CGAL: ' + str(filepath) + ' ...')
#                pygalmesh.
                mesh = pygalmesh.generate_volume_mesh_from_surface_mesh(filepath,
                                                                        lloyd=Lloyd,
                                                                        odt=ODT,
                                                                        perturb=Perturb,
                                                                        exude=Exude,
                                                                        facet_angle=Facet_Angle,
                                                                        facet_size=Facet_Size,
                                                                        facet_distance=Facet_Distance,
                                                                        cell_radius_edge_ratio=Cell_Radius_Edge_Ratio,
                                                                        cell_size=Cell_Size,
                                                                        verbose=True)
                
                meshio.write(OutputFilePath, mesh)
                logging.info(' Finished meshing.')   
    
            return OutputFilePath

def createScene(rootNode):
        """
        Example scene in SOFA, where the parametric mesh generator is used generate a volumetric mesh from a STEP-file while passing some options to Gmsh.          
        """ 
        import Sofa.Core
        import Sofa.Simulation
        import SofaRuntime
        import os
        
        logging.basicConfig(level=logging.INFO)
        
        MeshesPath = os.path.dirname(os.path.abspath(__file__))+'/data/meshes/'
        
        SofaRuntime.importPlugin("SofaPython3")
        SofaRuntime.importPlugin("SoftRobots")
        SofaRuntime.importPlugin("SoftRobots.Inverse")
        
        rootNode.addObject('VisualStyle', displayFlags='showForceFields showInteractionForceFields')

        rootNode.gravity = [0, 0, -9180]
        rootNode.dt = 1

        rootNode.addObject('LCPConstraintSolver')
        rootNode.addObject('FreeMotionAnimationLoop')
        rootNode.addObject('BackgroundSetting', color='0 0.168627 0.211765')
        rootNode.addObject('OglSceneFrame', style="Arrows", alignment="TopRight")

        # See the code above for all possible parameters
        # Example mesh is in mili-meters, parameters for meters will probably be different!
        filename = meshVolumetricFromSurface(filepath=MeshesPath+'surface_mesh_example.stl',
                                             outputdir=MeshesPath+'autogen/',
                                             Cell_Radius_Edge_Ratio=3,
                                             Cell_Size=4,
                                             Edge_Size=1,
                                             Facet_Angle=20,
                                             Facet_Distance=0.5)
                                      
        rootNode.addObject("MeshVTKLoader", name="loader", filename=filename)
        rootNode.addObject("TetrahedronSetTopologyContainer", name="container", src="@loader")

        rootNode.addObject("MechanicalObject", name="dofs", position="@loader.position")
        rootNode.addObject("TetrahedronFEMForceField", name="forcefield")                              


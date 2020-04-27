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
 
def meshFromParametricGeometry(filepath, outputdir='autogen/', meshtype='Surface', **kwargs):
        """
        generate a tetrahedron and/or a surface mesh from the provided file and store the 
        result in a volumetric (vtk) or surfacic file (stl, etc.). The path to the file is returned. 
           
        :param str filepath: 
            The path to the file (including extension) to the parametric mesh.
        :param str outputdir:
            The path to the directory where the meshed file will be stored.
        :param str meshtype:
            This accepts the values 'Surface' or 'Volumetric', i.e. the type of mesh to generate from the source
        
        :param dict **kwargs:
          
            This function accepts through the kwargs all options that can be set in Gmsh as found in the documentation:
            http://gmsh.info/doc/texinfo/gmsh.html. The options in the referenced documentation are sorted into categories ("Mesh", "Geometry", etc.) and separated by a point, e.g. "Mesh.CharacteristicLengthFactor". 
            The same option passed as kwarg is separated by an underscore, e.g. "Mesh_CharacteristicLengthFactor=0.8". See example below.
        """ 
        import splib.caching.cacher as cch
        import gmsh      
        import os
        import numpy as np
        #import locale        
        #locale.setlocale(locale.LC_ALL, "C")
        
        with LocaleManager('C'):       
            # Set options from kwargs           
            OptionsStrings, Values = cch.extractOptions(kwargs)        
            
            gmsh.initialize()
            for i in range(0, len(OptionsStrings)):            
                SplitStr = OptionsStrings[i].split('_')
                Category = SplitStr[0]
                Option = SplitStr[1]
                GmshOptionStr = Category+'.'+Option 
                if isinstance(Values[i], str):  # need to be careful to call the correct function according to the type of value (string or numerical)
                    gmsh.option.setString(GmshOptionStr, Values[i])
                else:
                    gmsh.option.setNumber(GmshOptionStr, Values[i])                
                
            Refresh = False
            OutputFilePath = ''        
            FileNameWithExt = os.path.split(filepath)[1]
            FileNameNoExt = os.path.splitext(FileNameWithExt)[0]                
            
            #Refresh, OutputFilePath = casher(filepath, outputdir, '.stl', kwargs, FileNameNoExt)
                        
            if meshtype == 'Surface':            
                Refresh, OutputFilePath = cch.cacher(filepath, outputdir, '.stl', kwargs, FileNameNoExt)
            elif meshtype == 'Volumetric':            
                Refresh, OutputFilePath = cch.cacher(filepath, outputdir, '.vtk', kwargs, FileNameNoExt)
            
    
            if Refresh:
                logging.info(' Beginning meshing ' + str(filepath) + ' ...')
                gmsh.initialize()
                gmsh.open(filepath)
                if meshtype == 'Surface':
                    gmsh.model.mesh.generate(2)
                elif meshtype == 'Volumetric':
                    gmsh.model.mesh.generate(3)    
                gmsh.write(OutputFilePath)
                logging.info(' Finished meshing.')   
    
            gmsh.finalize()
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

        # The list of mesh (e.g. Mesh_CharacteristicLengthFactor), geometry, view, etc. options can be found here: http://gmsh.info/doc/texinfo/gmsh.html, Appendix B
        filename = meshFromParametricGeometry(filepath=MeshesPath+'parametric_mesh_example.step', 
                                      outputdir=MeshesPath+'autogen/',
                                      meshtype='Volumetric',
                                      Mesh_CharacteristicLengthFactor=1, 
                                      Mesh_CharacteristicLengthMax=3, 
                                      Mesh_CharacteristicLengthMin=0.1, 
                                      View_GeneralizedRaiseZ='v0')
                                      
        rootNode.addObject("MeshVTKLoader", name="loader", filename=filename)
        rootNode.addObject("TetrahedronSetTopologyContainer", name="container", src="@loader")

        rootNode.addObject("MechanicalObject", name="dofs", position="@loader.position")
        rootNode.addObject("TetrahedronFEMForceField", name="forcefield")                              


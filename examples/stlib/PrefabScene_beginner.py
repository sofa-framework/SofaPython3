from stlib.entities.deformable import Deformable

def createScene(root):
    params = Deformable.getParameters()
    params.name = "Logo"
    params.topology.filename="share/mesh/SOFA-logo.obj"
    root.add(Deformable, params=params)     
    return root

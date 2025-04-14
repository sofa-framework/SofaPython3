from splib.helper import exportScene
from stlib.misc.entity import Entity


def createScene(root):
    params = Entity.Deformable.Parameters()
    params.name = "Logo"
    params.topology.filename="share/mesh/SOFA-logo.obj"
    root.add(Entity.Deformable, params=params)     
    return root


if __name__=="__main__":
    root = exportScene()
    createScene(root)
    pass
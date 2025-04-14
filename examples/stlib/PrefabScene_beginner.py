from splib.helper import exportScene
from stlib.simulated.deformable import Deformable


def createScene(root):
    params = Deformable.Parameters()
    params.name = "Logo"
    params.topology.filename="share/mesh/SOFA-logo.obj"
    root.add(Deformable, params=params)     
    return root


if __name__=="__main__":
    root = exportScene()
    createScene(root)
    pass
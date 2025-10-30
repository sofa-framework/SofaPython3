from cristal.prefabs.interactivecamera import InteractiveCamera
from cristal.scenes.rooms import Office220
import SofaRuntime

def createScene(root):
    import SofaRuntime
    SofaRuntime.DataRepository.addLastPath("/home/dmarchal/projects/dev/mega-assets/")
    SofaRuntime.DataRepository.addLastPath("/home/dmarchal/projects/dev/sofa1/plugins/Sofa.PointCloud/examples/assets")

    camera = root.add(InteractiveCamera, name="Camera")
    camera.state.computeZClip = False
    camera.state.zNear = 0.0001
    camera.state.zFar = 1000000
    
    root.add(Office220, name="Office220")

    # We need to connect the camera to the renderer.
    root.add("PointCloudRenderer", name="renderer",
                                   geometry = root.Office220.container.linkpath,
                                   camera = camera.state.linkpath)

from stlib.core.basePrefab import BasePrefab
from stlib.core.baseParameters import BaseParameters

class Office220(BasePrefab):
    def __init__(self, parameters : BaseParameters = None, **kwargs):
        if parameters is None:
            parameters = BaseParameters(**kwargs)
        BasePrefab.__init__(self, parameters)
        self.__create_prefab__()

    def __create_prefab__(self):
        self.getRoot().add("RequiredPlugin", name="Sofa.PointCloud")
        
        file = "splats/defrost/room/office220.ply"
        #file = "splats/cristal/room/salle1/salle-v2.ply"
        #file = "splats/pretil/limo/limo.ply"
        #file = "splats/pretil/limo/limo-cleaned.ply"
        self.add("PointCloudContainer", name="loader", filename=file)
        self.add("PointCloudContainer", name="container", filename=file)
        
        self.add("PointCloudTransform", name="transform", 
                                        input=self.loader.linkpath, 
                                        output=self.container.linkpath, scale=[1,1.0,1.0])
class SoftRobots:
    class Cable(Sofa.Core.BasePrefab):
        length : float 

        def __init__(self,**kwargs):
            pass

        def Parameters(object):
            lenght : float    

class Trunk(Sofa.Core.BasePrefab):
    body : Entity.Deformable
    cables : list [SoftRobots.Cable]
    
    def __init__(self, params):
        body = Entity.Deformable()

        for param in range(params.cables):
            cables.append(SoftRobots.Cable(body, param))

    class Parameters(object):
        body : Entity.Deformable.Parameters
        cables : list[SoftRobots.Cable.Parameters]

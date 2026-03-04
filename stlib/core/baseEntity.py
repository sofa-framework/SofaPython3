from stlib.core.basePrefab import BasePrefab
from stlib.core.baseParameters import BaseParameters

class BaseEntity(BasePrefab):

    def __init__(self, parameters: BaseParameters):
        BasePrefab.__init__(self, parameters=parameters)

        

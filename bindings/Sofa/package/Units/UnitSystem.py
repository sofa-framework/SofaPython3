from .Core import *
from .Definitions import DimensionLess, s, m, kg
import numpy as np

class UnitSystem():

    units : dict

    def __init__(self, *args, **kwargs):
        self.units = {}
        for arg in args:
            self.setPrimaryUnit(arg)
        for arg in kwargs:
            self.setPrimaryUnit(kwargs[arg])
    
    def setPrimaryUnit(self, unit):
        if isinstance(unit, Unit):
                if len(unit.numerator) == 1 and len(unit.denumerator) == 0:
                    if unit.numerator[0].abrev not in self.units:
                        self.units[unit.numerator[0].abrev] = unit
                    else:
                        raise ValueError("Only one primary unit of each type can be defined")
                else:
                    raise TypeError("Only primary unit (with an optionnal ratio) can be defined by the user.")

    def convert(self, value : float, unit: DerivedUnit):
        u_key = unit.getKey()

        reconstructedUnit = DimensionLess
        for nkey in u_key["num"]:
            try:
                for _ in range(u_key["num"][nkey]):
                    reconstructedUnit *= self.units[nkey]
            except:
                raise RuntimeError(f"The unit {nkey} is not defined in the parameter set.")
        for nkey in u_key["denum"]:
            try:
                for _ in range(u_key["denum"][nkey]):
                    reconstructedUnit /= self.units[nkey]
            except:
                raise RuntimeError(f"The unit {nkey} is not defined in the parameter set.")


        return unit.ratio / reconstructedUnit.ratio * value
    

    def __call__(self, *args):
        if len(args) == 1:
            if isinstance(args[0], np.ndarray):
                convertedArray = np.empty(args[0].shape, dtype=np.float32)
                for i in range(convertedArray.size):
                    convertedArray.flat[i] = self.convert(value=args[0].flat[i].value, unit= args[0].flat[i].unit)
                return convertedArray
            elif isinstance(args[0], list):
                retList = [None] * len(args[0])
                for i in range(len(retList)):
                    retList[i] = self.__call__(args[0][i])
                return retList
            else:
                return self.convert(value=args[0].value, unit= args[0].unit)
        elif len(args) == 2:
            if isinstance(args[0], np.ndarray):
                convertedArray = np.empty(args[0].shape, dtype=np.float32)
                for i in range(convertedArray.size):
                    convertedArray.flat[i] = self.convert(value=args[0].flat[i], unit = args[1])
                return convertedArray
            elif isinstance(args[0], list):
                retList = [None] * len(args[0])
                for i in range(len(retList)):
                    retList[i] = self.__call__(args[0][i], args[1])
                return retList
                
            else:
                return self.convert(value=args[0], unit= args[1])
        else:
            raise ValueError("This method requires either a DimensionnedValue as input or a float and a Unit.")
    
    
    
    
                

class MechanicalUnitSystem(UnitSystem):
    def __init__(self, time = s, length = m, mass = kg ):
        UnitSystem.__init__(self, time, length, mass)
        


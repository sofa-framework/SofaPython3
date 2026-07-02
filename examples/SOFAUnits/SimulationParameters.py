
from units import *

class BaseParameterSet():

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
            return self.convert(value=args[0].value, unit= args[0].unit)
        elif len(args) == 2:
            return self.convert(value=args[0], unit= args[1])
        else:
            raise ValueError("This method requires either a DimensionnedValue as input or a float and a Unit.")
    
    
    
    
                

class SOFAParameters(BaseParameterSet):
    def __init__(self, time = s, length = m, mass = kg ):
        BaseParameterSet.__init__(self, time, length, mass)
        


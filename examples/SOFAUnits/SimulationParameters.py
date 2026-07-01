
from units import *

class BaseParameterSet():

    units : dict

    def __init__(self, *args, **kwargs):
        self.units = {}
        for arg in args:
            if isinstance(arg, Unit):
                if len(arg.numerator) == 1 and len(arg.denumerator) == 0:
                    self.units[arg.numerator[0].abrev] = arg
                else:
                    raise TypeError("Only primary unit (with an optionnal ratio) can be defined by the user.")
        for arg in kwargs:
            if isinstance(kwargs[arg], Unit):
                if len(kwargs[arg].numerator) == 1 and len(kwargs[arg].denumerator) == 0:
                    self.units[kwargs[arg].numerator[0].abrev] = kwargs[arg]
                else:
                    raise TypeError("Only primary unit (with an optionnal ratio) can be defined by the user.")
    
    def convert(self, value : float, unit: DerivedUnit):
        u_key = unit.getKey()

        reconstructedUnit = neutralUnit
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
    
    def __call__(self, value : float, unit: DerivedUnit):
        return self.convert(value=value, unit= unit)
    
                

class SOFAParameters(BaseParameterSet):
    def __init__(self, time = s, position = m, mass = kg ):
        BaseParameterSet.__init__(self, time, position, mass)
        self.time = time
        self.position = position
        self.mass = mass
        

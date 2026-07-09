import math

class Unit():
    numerator : list
    denumerator : list
    ratio : float


    def getKey(self):
        key = {"num" : {}, "denum" : {}}
        for unit in self.numerator:
            if unit.abrev in key["num"]:
                key["num"][unit.abrev] += 1
            else:
                key["num"][unit.abrev] = 1
        for unit in self.denumerator:
            if unit.abrev in key["denum"]:
                key["denum"][unit.abrev] += 1
            else:
                key["denum"][unit.abrev] = 1
        return key
    
    def __eq__ (self, other):
        if not isinstance(other, Unit):
            return NotImplemented

        if int(math.log10(self.ratio)) != int(math.log10(other.ratio)) :
            return False
        
        return self.getKey() == other.getKey() 


    def __mul__(self, other):
        if isinstance(other, Unit):
            return DerivedUnit(numerator=self.numerator + other.numerator, denumerator= self.denumerator + other.denumerator, ratio = self.ratio * other.ratio)
        else:
            return DimensionnedValue(other,self )
        

    def __rmul__(self, other ):
        return self.__mul__(other)
    
    def __pow__(self, other : int):
        if not isinstance(other, int):
            raise ValueError

        targetNum = []
        targetDenum = []
        targetRatio = 1.0

        for i in range(abs(other)):
            targetNum += self.numerator
            targetDenum += self.denumerator
            targetRatio *= self.ratio
        
        if other < 0 :
            return DerivedUnit(numerator=targetDenum, denumerator= targetNum, ratio = 1.0/targetRatio)
        elif other > 0 :
            return DerivedUnit(numerator=targetNum, denumerator= targetDenum, ratio = targetRatio)
        else:
            return NeutralUnit()

    def __truediv__(self, other ):
        if not isinstance(other, Unit):
            return NotImplemented
        
        return DerivedUnit(numerator=self.numerator + other.denumerator, denumerator= self.denumerator + other.numerator, ratio = self.ratio / other.ratio)

    def toString(self, addRatio : bool = True):
        self_key = self.getKey()

        def side(units: dict) -> str:
            return " * ".join(
                k if exp == 1 else f"{k}^{exp}"
                for k, exp in units.items()
            )

        num = side(self_key["num"])
        denum = side(self_key["denum"])

        num_s = f"( {num} ) " if num else "1"
        denum_s = f"/ ( {denum} )" if denum else ""

        prefix = f"{self.ratio} * " if addRatio else ""
        return prefix + num_s + denum_s


    def __str__(self):
        return self.toString()
    
    def __hash__(self):
        key = self.getKey()
        return hash((
            frozenset(key["num"].items()),
            frozenset(key["denum"].items()),
            int(math.log10(self.ratio)),
        ))

class NeutralUnit(Unit):
    def __init__(self):
        self.numerator = []
        self.denumerator = []
        self.ratio = 1.0
        
    def __str__(self):
        return "1"
    

class PrimaryUnit(Unit):

    abrev = str

    def __init__(self, abrev : str):
        self.abrev = abrev
        self.numerator = [self]
        self.denumerator = []
        self.ratio = 1.0



class DerivedUnit(Unit):

    def __init__(self, numerator : list[PrimaryUnit], denumerator : list[PrimaryUnit], ratio : float):
        self.numerator = numerator  
        self.denumerator = denumerator  
        self.ratio = ratio

        self.simplify()


    def simplify(self):
        futNum = []
        for unit in self.numerator:
            simplified = False
            for i in range(len(self.denumerator)):
                if self.denumerator[i].abrev == unit.abrev:
                    simplified = True
                    self.denumerator.pop(i)
                    break
            if not(simplified):
                futNum.append(unit)
        self.numerator = futNum
        

class ScaledUnit(Unit):

    def __init__(self, unit : Unit, ratio : float):
        self.numerator = unit.numerator.copy()
        self.denumerator = unit.denumerator.copy()
        self.ratio = ratio


class DimensionnedValue():

    value : float 
    unit : Unit

    def __init__(self, value : float, unit : Unit):
        self.value = value
        self.unit = unit

    def __eq__ (self, other):
        if not isinstance(other, DimensionnedValue):
            raise TypeError("Dimensionned values can only be compared to other dimensionned values")
    

        if self.unit.getKey() != other.unit.getKey():
            raise TypeError("Only values that share the same units can be compared")

        return math.isclose(self.value * self.unit.ratio, other.value * other.unit.ratio)


    def __mul__(self, other):
        if isinstance(other, DimensionnedValue):
            return DimensionnedValue(self.value * other.value,self.unit * other.unit)
        elif isinstance(other, Unit) :
            return DimensionnedValue(self.value ,self.unit * other)
        else :
            return DimensionnedValue(self.value * other,self.unit)

    def __rmul__(self, other ):
        return self.__mul__(other)
    
    def __pow__(self, other : int):
        if not isinstance(other, int):
            raise ValueError

        return DimensionnedValue(self.value ** other, self.unit**other)

    def __truediv__(self, other):
        if isinstance(other, DimensionnedValue):
            return DimensionnedValue(self.value / other.value, self.unit / other.unit)
        elif isinstance(other, Unit) :
            return DimensionnedValue(self.value, self.unit / other)
        else:
            return DimensionnedValue(self.value / other, self.unit)
    
    def __rtruediv__(self, other):
        if isinstance(other, DimensionnedValue):
            return DimensionnedValue(other.value / self.value, other.unit / self.unit)
        elif isinstance(other, Unit):
            return DimensionnedValue(1.0 / self.value, other / self.unit)
        else:
            return DimensionnedValue(other / self.value, self.unit ** -1)
        
    def __str__(self):
        return f"{self.value * self.unit.ratio} * " + self.unit.toString(False)
        
    def __hash__(self):
        key = self.unit.getKey()
        return hash((
            frozenset(key["num"].items()),
            frozenset(key["denum"].items()),
            round(self.value * self.unit.ratio, 9)     # normalized magnitude
        ))


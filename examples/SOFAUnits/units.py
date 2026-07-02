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
            raise TypeError

        if int(math.log10(self.ratio)) != int(math.log10(other.ratio)) :
            return False
        
        s_key = self.getKey()
        o_key = other.getKey()

        isEq = True

        for dicName in ["num", "denum"]:
            for key in s_key[dicName]:
                isEq = isEq and (key in o_key[dicName]) and (s_key[dicName][key] == o_key[dicName][key] )

        return isEq 


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
            return NeutralUnit

    def __truediv__(self, other ):
        if not isinstance(other, Unit):
            raise TypeError
        
        return DerivedUnit(numerator=self.numerator + other.denumerator, denumerator= self.denumerator + other.numerator, ratio = self.ratio / other.ratio)

    def toString(self, addRatio : bool = True):
        
        self_key = self.getKey()
        num_s = "( "
        first = True
        for key in self_key["num"]:
            if not first:
                num_s+=" * "
            first = False
            num_s += key
            if self_key["num"][key] != 1:
                exposant = self_key["num"][key]
                num_s+=f"^{exposant}"
            
        if len(num_s) != 2:
            num_s += " ) "
        else:
            num_s = "1"

        
        denum_s = "/ ( "
        first = True
        for key in self_key["denum"]:
            if not first:
                denum_s+=" * "
            first = False
            denum_s += key
            if self_key["denum"][key] != 1:
                exposant = self_key["denum"][key]
                denum_s+=f"^{exposant}"
            

        if len(num_s) != 4:
            denum_s += " )"
        else:
            denum_s = ""


        if addRatio:
            return f"{self.ratio} * " + num_s + denum_s
        else:
            return num_s + denum_s


    def __str__(self):
        return self.toString()

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
        
        no_ratio_s_unit = self.unit
        no_ratio_s_unit.ratio = 1.0
        no_ratio_o_unit = other
        no_ratio_o_unit.ratio = 1.0

        if not no_ratio_s_unit == no_ratio_o_unit:
            raise TypeError("Only values that share the same units can be compared")

        return self.value * self.unit.ration == other.value * other.unit.ratio


    def __mul__(self, other):
        if isinstance(other, DimensionnedValue):
            return DimensionnedValue(self.value * other.value,self.unit * other.unit)
        elif isinstance(other, Unit) :
            return DimensionnedValue(self.value ,self.unit * other )
        else :
            return DimensionnedValue(self.value * other.value,self.unit )

    def __rmul__(self, other ):
        return self.__mul__(other)
    
    def __pow__(self, other : int):
        if not isinstance(other, int):
            raise ValueError

        return DimensionnedValue(self.value ** other , self.unit**other )

    def __truediv__(self, other ):
        if isinstance(other, DimensionnedValue):
            return DimensionnedValue(self.value / other.value,self.unit / other.unit)
        elif isinstance(other, Unit) :
            return DimensionnedValue(self.value ,self.unit / other )
        else:
            return DimensionnedValue(self.value / other.value,self.unit )
        
    def __str__(self):
        return f"{self.value * self.unit.ratio} * " + self.unit.toString(False)
        



### Primary units
DimensionLess = NeutralUnit()
s = PrimaryUnit("s")        # time
m = PrimaryUnit("m")        # length
kg = PrimaryUnit("kg")      # mass
A = PrimaryUnit("A")        # electric current
K = PrimaryUnit("K")        # temperature
mol = PrimaryUnit("mol")    # amount of substance
cd = PrimaryUnit("cd")      # luminous intensity


### (some) Derived units
v = m/s                     # velocity
a = v/s                     # acceleration
N = kg*a                    # force (Newton)
Pa = N/(m**2)               # pressure (Pascal)
tho = m*N                   # torque 
J = kg*m**2/s**2            # energy (Joule)
W = J/s                     # power (Watt)


## Scaled primary units
nm = ScaledUnit(m, 1e-9)
µm = ScaledUnit(m, 1e-6)
mm = ScaledUnit(m, 1e-3)
cm = ScaledUnit(m, 1e-2)
dm = ScaledUnit(m, 1e-1)
km = ScaledUnit(m, 1e3)

ns = ScaledUnit(s, 1e-9)
µs = ScaledUnit(s, 1e-6)
ms = ScaledUnit(s, 1e-3)

µg = ScaledUnit(kg, 1e-9)
mg = ScaledUnit(kg, 1e-6)
g = ScaledUnit(kg, 1e-3)
t = ScaledUnit(kg, 1e3)

## Scaled derived units
nN = ScaledUnit(N, 1e-9)
µN = ScaledUnit(N, 1e-6)
mN = ScaledUnit(N, 1e-3)
cN = ScaledUnit(N, 1e-2)
dN = ScaledUnit(N, 1e-1)
kN = ScaledUnit(N, 1e3)
MN = ScaledUnit(N, 1e6)
GN = ScaledUnit(N, 1e9)


nPa = ScaledUnit(Pa, 1e-9)
µPa = ScaledUnit(Pa, 1e-6)
mPa = ScaledUnit(Pa, 1e-3)
cPa = ScaledUnit(Pa, 1e-2)
dPa = ScaledUnit(Pa, 1e-1)
kPa = ScaledUnit(Pa, 1e3)
MPa = ScaledUnit(Pa, 1e6)
GPa = ScaledUnit(Pa, 1e9)

mJ = ScaledUnit(J, 1e-3)
cJ = ScaledUnit(J, 1e-2)
dJ = ScaledUnit(J, 1e-1)
kJ = ScaledUnit(J, 1e3)
MJ = ScaledUnit(J, 1e6)
GJ = ScaledUnit(J, 1e9)

mW = ScaledUnit(W, 1e-3)
cW = ScaledUnit(W, 1e-2)
dW = ScaledUnit(W, 1e-1)
kW = ScaledUnit(W, 1e3)
MW = ScaledUnit(W, 1e6)
GW = ScaledUnit(W, 1e9)






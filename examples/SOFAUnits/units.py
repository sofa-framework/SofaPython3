import math

class Unit():
    numerator : list
    denumerator : list
    ratio : float

    def printReduced(self):
        self_key = self.getKey()
        num_s = "( "
        first = True
        for key in self_key["num"]:
            if not first:
                num_s+=" * "
            first = False
            num_s += key
            if self_key["num"][key] != 1:
                num_s+=f"^{self_key["num"][key]}"
            
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
                denum_s+=f"^{self_key["denum"][key]}"
            
        if len(num_s) != 4:
            denum_s += " )"
        else:
            denum_s = ""


        print(f"{self.ratio} * " + num_s + denum_s)

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
        return DerivedUnit(numerator=self.numerator + other.numerator, denumerator= self.denumerator + other.denumerator, ratio = self.ratio * other.ratio)

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
            return DerivedUnit(numerator=self.numerator + other.denumerator, denumerator= self.denumerator + other.numerator, ratio = self.ratio / other.ratio)




class NeutralUnit(Unit):
    def __init__(self):
        self.numerator = []
        self.denumerator = []
        self.ratio = 1.0
    

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



### Primary units
neutralUnit = NeutralUnit()
m = PrimaryUnit("m")
s = PrimaryUnit("s")
kg = PrimaryUnit("kg")


### Derived units
v = m/s
a = v/s
N = kg*a
Pa = N/(m**2)
tho = m*N


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






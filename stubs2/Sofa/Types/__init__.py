"""
           Default types available in Sofa
           -----------------------

           .. autosummary::
               Sofa.Types.Vec3
               Sofa.Types.BoundingBox
               ...

       """
import Sofa.Types
from typing import *
from typing import Iterable as iterable
from typing import Iterator as iterator
from numpy import float64
_Shape = Tuple[int, ...]
__all__  = [
"BoundingBox"
]
class BoundingBox():
    def getCenter(self) -> 'list': 
        """
returns the center of the bbox
"""
    def getMax(self) -> 'list': 
        """
Returns a list (by copy) of the max bbox position
"""
    def getMin(self) -> 'list': 
        """
Returns a list (by copy) of the min bbox position
"""
    def setMax(self, arg0: list) -> 'None': 
        """
sets the max bbox position from a python list
"""
    def setMin(self, arg0: list) -> 'None': 
        """
sets the min bbox position from a python list
"""
    pass
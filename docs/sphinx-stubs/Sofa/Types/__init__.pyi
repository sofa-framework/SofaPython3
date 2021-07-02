"""
           Default data types
           -------------------------------

           .. autosummary::
               Sofa.Types.BoundingBox
               ...

       """
import Sofa.Types
import typing
import Sofa.Core

__all__ = [
    "BoundingBox"
]


class BoundingBox(Sofa.Core.Data):
    def getCenter(self) -> list: 
        """
        returns the center of the bbox
        """
    def getMax(self) -> list: 
        """
        Returns a list (by copy) of the max bbox position
        """
    def getMin(self) -> list: 
        """
        Returns a list (by copy) of the min bbox position
        """
    def setMax(self, arg0: list) -> None: 
        """
        sets the max bbox position from a python list
        """
    def setMin(self, arg0: list) -> None: 
        """
        sets the min bbox position from a python list
        """
    pass

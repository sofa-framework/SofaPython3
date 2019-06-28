"""
           Utility functions
           -----------------------

           .. autosummary::
               Sofa.Helper.msg_info
               Sofa.Helper.msg_warning
               Sofa.Helper.msg_error
               Sofa.Helper.msg_deprecated
               Sofa.Helper.msg_fatal

       """
import Sofa.Helper
from typing import *
from typing import Iterable as iterable
from typing import Iterator as iterator
from numpy import float64
_Shape = Tuple[int, ...]
import Sofa.Core
__all__  = [
"StdVectorBaseData",
"StdVectordouble",
"StdVectorfloat",
"StdVectorint",
"StdVectorlong",
"StdVectoruint",
"StdVectorulong",
"VectorBaseData",
"Vectordouble",
"Vectorfloat",
"Vectorint",
"Vectorlong",
"Vectoruint",
"Vectorulong",
"msg_deprecated",
"msg_error",
"msg_fatal",
"msg_info",
"msg_warning"
]
class StdVectorBaseData():
    def __bool__(self) -> 'bool': 
        """
Check whether the list is nonempty
"""
    def __contains__(self, x: Sofa.Core.Data) -> 'bool': 
        """
Return true the container contains ``x``
"""
    @overload
    def __delitem__(self, arg0: int) -> 'None': 
        pass
    @overload
    def __delitem__(self, arg0: slice) -> 'None': ...
    def __eq__(self, arg0: StdVectorBaseData) -> 'bool': ...
    @overload
    def __getitem__(self, s: slice) -> 'StdVectorBaseData': 
        pass
    @overload
    def __getitem__(self, arg0: int) -> 'Sofa.Core.Data': ...
    @overload
    def __init__(self) -> 'None': 
        pass
    @overload
    def __init__(self, arg0: StdVectorBaseData) -> 'None': ...
    @overload
    def __init__(self, arg0: object) -> 'None': ...
    def __iter__(self) -> 'object': ...
    def __len__(self) -> 'int': ...
    def __ne__(self, arg0: StdVectorBaseData) -> 'bool': ...
    def __repr__(self) -> 'str': 
        """
Return the canonical string representation of this list.
"""
    @overload
    def __setitem__(self, arg0: int, arg1: Sofa.Core.Data) -> 'None': 
        pass
    @overload
    def __setitem__(self, arg0: slice, arg1: StdVectorBaseData) -> 'None': ...
    def append(self, x: Sofa.Core.Data) -> 'None': 
        """
Add an item to the end of the list
"""
    def count(self, x: Sofa.Core.Data) -> 'int': 
        """
Return the number of times ``x`` appears in the list
"""
    def extend(self, L: StdVectorBaseData) -> 'None': 
        """
Extend the list by appending all the items in the given list
"""
    def insert(self, i: int, x: Sofa.Core.Data) -> 'None': 
        """
Insert an item at a given position.
"""
    @overload
    def pop(self, i: int) -> 'Sofa.Core.Data': 
        pass
    @overload
    def pop(self) -> 'Sofa.Core.Data': ...
    def remove(self, x: Sofa.Core.Data) -> 'None': 
        """
Remove the first item from the list whose value is x. It is an error if there is no such item.
"""
    pass
class StdVectordouble():
    def __bool__(self) -> 'bool': 
        """
Check whether the list is nonempty
"""
    def __contains__(self, x: float) -> 'bool': 
        """
Return true the container contains ``x``
"""
    @overload
    def __delitem__(self, arg0: slice) -> 'None': 
        pass
    @overload
    def __delitem__(self, arg0: int) -> 'None': ...
    def __eq__(self, arg0: StdVectordouble) -> 'bool': ...
    @overload
    def __getitem__(self, s: slice) -> 'StdVectordouble': 
        pass
    @overload
    def __getitem__(self, arg0: int) -> 'float': ...
    @overload
    def __init__(self) -> 'None': 
        pass
    @overload
    def __init__(self, arg0: object) -> 'None': ...
    @overload
    def __init__(self, arg0: StdVectordouble) -> 'None': ...
    def __iter__(self) -> 'object': ...
    def __len__(self) -> 'int': ...
    def __ne__(self, arg0: StdVectordouble) -> 'bool': ...
    def __repr__(self) -> 'str': 
        """
Return the canonical string representation of this list.
"""
    @overload
    def __setitem__(self, arg0: int, arg1: float) -> 'None': 
        pass
    @overload
    def __setitem__(self, arg0: slice, arg1: StdVectordouble) -> 'None': ...
    def append(self, x: float) -> 'None': 
        """
Add an item to the end of the list
"""
    def count(self, x: float) -> 'int': 
        """
Return the number of times ``x`` appears in the list
"""
    def extend(self, L: StdVectordouble) -> 'None': 
        """
Extend the list by appending all the items in the given list
"""
    def insert(self, i: int, x: float) -> 'None': 
        """
Insert an item at a given position.
"""
    @overload
    def pop(self) -> 'float': 
        pass
    @overload
    def pop(self, i: int) -> 'float': ...
    def remove(self, x: float) -> 'None': 
        """
Remove the first item from the list whose value is x. It is an error if there is no such item.
"""
    pass
class StdVectorfloat():
    def __bool__(self) -> 'bool': 
        """
Check whether the list is nonempty
"""
    def __contains__(self, x: float) -> 'bool': 
        """
Return true the container contains ``x``
"""
    @overload
    def __delitem__(self, arg0: int) -> 'None': 
        pass
    @overload
    def __delitem__(self, arg0: slice) -> 'None': ...
    def __eq__(self, arg0: StdVectorfloat) -> 'bool': ...
    @overload
    def __getitem__(self, s: slice) -> 'StdVectorfloat': 
        pass
    @overload
    def __getitem__(self, arg0: int) -> 'float': ...
    @overload
    def __init__(self, arg0: StdVectorfloat) -> 'None': 
        pass
    @overload
    def __init__(self, arg0: object) -> 'None': ...
    @overload
    def __init__(self) -> 'None': ...
    def __iter__(self) -> 'object': ...
    def __len__(self) -> 'int': ...
    def __ne__(self, arg0: StdVectorfloat) -> 'bool': ...
    def __repr__(self) -> 'str': 
        """
Return the canonical string representation of this list.
"""
    @overload
    def __setitem__(self, arg0: slice, arg1: StdVectorfloat) -> 'None': 
        pass
    @overload
    def __setitem__(self, arg0: int, arg1: float) -> 'None': ...
    def append(self, x: float) -> 'None': 
        """
Add an item to the end of the list
"""
    def count(self, x: float) -> 'int': 
        """
Return the number of times ``x`` appears in the list
"""
    def extend(self, L: StdVectorfloat) -> 'None': 
        """
Extend the list by appending all the items in the given list
"""
    def insert(self, i: int, x: float) -> 'None': 
        """
Insert an item at a given position.
"""
    @overload
    def pop(self) -> 'float': 
        pass
    @overload
    def pop(self, i: int) -> 'float': ...
    def remove(self, x: float) -> 'None': 
        """
Remove the first item from the list whose value is x. It is an error if there is no such item.
"""
    pass
class StdVectorint():
    def __bool__(self) -> 'bool': 
        """
Check whether the list is nonempty
"""
    def __contains__(self, x: int) -> 'bool': 
        """
Return true the container contains ``x``
"""
    @overload
    def __delitem__(self, arg0: int) -> 'None': 
        pass
    @overload
    def __delitem__(self, arg0: slice) -> 'None': ...
    def __eq__(self, arg0: StdVectorint) -> 'bool': ...
    @overload
    def __getitem__(self, arg0: int) -> 'int': 
        pass
    @overload
    def __getitem__(self, s: slice) -> 'StdVectorint': ...
    @overload
    def __init__(self, arg0: object) -> 'None': 
        pass
    @overload
    def __init__(self, arg0: StdVectorint) -> 'None': ...
    @overload
    def __init__(self) -> 'None': ...
    def __iter__(self) -> 'object': ...
    def __len__(self) -> 'int': ...
    def __ne__(self, arg0: StdVectorint) -> 'bool': ...
    def __repr__(self) -> 'str': 
        """
Return the canonical string representation of this list.
"""
    @overload
    def __setitem__(self, arg0: slice, arg1: StdVectorint) -> 'None': 
        pass
    @overload
    def __setitem__(self, arg0: int, arg1: int) -> 'None': ...
    def append(self, x: int) -> 'None': 
        """
Add an item to the end of the list
"""
    def count(self, x: int) -> 'int': 
        """
Return the number of times ``x`` appears in the list
"""
    def extend(self, L: StdVectorint) -> 'None': 
        """
Extend the list by appending all the items in the given list
"""
    def insert(self, i: int, x: int) -> 'None': 
        """
Insert an item at a given position.
"""
    @overload
    def pop(self, i: int) -> 'int': 
        pass
    @overload
    def pop(self) -> 'int': ...
    def remove(self, x: int) -> 'None': 
        """
Remove the first item from the list whose value is x. It is an error if there is no such item.
"""
    pass
class StdVectorlong():
    def __bool__(self) -> 'bool': 
        """
Check whether the list is nonempty
"""
    def __contains__(self, x: int) -> 'bool': 
        """
Return true the container contains ``x``
"""
    @overload
    def __delitem__(self, arg0: int) -> 'None': 
        pass
    @overload
    def __delitem__(self, arg0: slice) -> 'None': ...
    def __eq__(self, arg0: StdVectorlong) -> 'bool': ...
    @overload
    def __getitem__(self, s: slice) -> 'StdVectorlong': 
        pass
    @overload
    def __getitem__(self, arg0: int) -> 'int': ...
    @overload
    def __init__(self, arg0: object) -> 'None': 
        pass
    @overload
    def __init__(self, arg0: StdVectorlong) -> 'None': ...
    @overload
    def __init__(self) -> 'None': ...
    def __iter__(self) -> 'object': ...
    def __len__(self) -> 'int': ...
    def __ne__(self, arg0: StdVectorlong) -> 'bool': ...
    def __repr__(self) -> 'str': 
        """
Return the canonical string representation of this list.
"""
    @overload
    def __setitem__(self, arg0: int, arg1: int) -> 'None': 
        pass
    @overload
    def __setitem__(self, arg0: slice, arg1: StdVectorlong) -> 'None': ...
    def append(self, x: int) -> 'None': 
        """
Add an item to the end of the list
"""
    def count(self, x: int) -> 'int': 
        """
Return the number of times ``x`` appears in the list
"""
    def extend(self, L: StdVectorlong) -> 'None': 
        """
Extend the list by appending all the items in the given list
"""
    def insert(self, i: int, x: int) -> 'None': 
        """
Insert an item at a given position.
"""
    @overload
    def pop(self, i: int) -> 'int': 
        pass
    @overload
    def pop(self) -> 'int': ...
    def remove(self, x: int) -> 'None': 
        """
Remove the first item from the list whose value is x. It is an error if there is no such item.
"""
    pass
class StdVectoruint():
    def __bool__(self) -> 'bool': 
        """
Check whether the list is nonempty
"""
    def __contains__(self, x: int) -> 'bool': 
        """
Return true the container contains ``x``
"""
    @overload
    def __delitem__(self, arg0: int) -> 'None': 
        pass
    @overload
    def __delitem__(self, arg0: slice) -> 'None': ...
    def __eq__(self, arg0: StdVectoruint) -> 'bool': ...
    @overload
    def __getitem__(self, arg0: int) -> 'int': 
        pass
    @overload
    def __getitem__(self, s: slice) -> 'StdVectoruint': ...
    @overload
    def __init__(self, arg0: object) -> 'None': 
        pass
    @overload
    def __init__(self) -> 'None': ...
    @overload
    def __init__(self, arg0: StdVectoruint) -> 'None': ...
    def __iter__(self) -> 'object': ...
    def __len__(self) -> 'int': ...
    def __ne__(self, arg0: StdVectoruint) -> 'bool': ...
    def __repr__(self) -> 'str': 
        """
Return the canonical string representation of this list.
"""
    @overload
    def __setitem__(self, arg0: slice, arg1: StdVectoruint) -> 'None': 
        pass
    @overload
    def __setitem__(self, arg0: int, arg1: int) -> 'None': ...
    def append(self, x: int) -> 'None': 
        """
Add an item to the end of the list
"""
    def count(self, x: int) -> 'int': 
        """
Return the number of times ``x`` appears in the list
"""
    def extend(self, L: StdVectoruint) -> 'None': 
        """
Extend the list by appending all the items in the given list
"""
    def insert(self, i: int, x: int) -> 'None': 
        """
Insert an item at a given position.
"""
    @overload
    def pop(self, i: int) -> 'int': 
        pass
    @overload
    def pop(self) -> 'int': ...
    def remove(self, x: int) -> 'None': 
        """
Remove the first item from the list whose value is x. It is an error if there is no such item.
"""
    pass
class StdVectorulong():
    def __bool__(self) -> 'bool': 
        """
Check whether the list is nonempty
"""
    def __contains__(self, x: int) -> 'bool': 
        """
Return true the container contains ``x``
"""
    @overload
    def __delitem__(self, arg0: int) -> 'None': 
        pass
    @overload
    def __delitem__(self, arg0: slice) -> 'None': ...
    def __eq__(self, arg0: StdVectorulong) -> 'bool': ...
    @overload
    def __getitem__(self, arg0: int) -> 'int': 
        pass
    @overload
    def __getitem__(self, s: slice) -> 'StdVectorulong': ...
    @overload
    def __init__(self, arg0: object) -> 'None': 
        pass
    @overload
    def __init__(self, arg0: StdVectorulong) -> 'None': ...
    @overload
    def __init__(self) -> 'None': ...
    def __iter__(self) -> 'object': ...
    def __len__(self) -> 'int': ...
    def __ne__(self, arg0: StdVectorulong) -> 'bool': ...
    def __repr__(self) -> 'str': 
        """
Return the canonical string representation of this list.
"""
    @overload
    def __setitem__(self, arg0: int, arg1: int) -> 'None': 
        pass
    @overload
    def __setitem__(self, arg0: slice, arg1: StdVectorulong) -> 'None': ...
    def append(self, x: int) -> 'None': 
        """
Add an item to the end of the list
"""
    def count(self, x: int) -> 'int': 
        """
Return the number of times ``x`` appears in the list
"""
    def extend(self, L: StdVectorulong) -> 'None': 
        """
Extend the list by appending all the items in the given list
"""
    def insert(self, i: int, x: int) -> 'None': 
        """
Insert an item at a given position.
"""
    @overload
    def pop(self, i: int) -> 'int': 
        pass
    @overload
    def pop(self) -> 'int': ...
    def remove(self, x: int) -> 'None': 
        """
Remove the first item from the list whose value is x. It is an error if there is no such item.
"""
    pass
class VectorBaseData(StdVectorBaseData):
    def __bool__(self) -> 'bool': 
        """
Check whether the list is nonempty
"""
    def __contains__(self, x: Sofa.Core.Data) -> 'bool': 
        """
Return true the container contains ``x``
"""
    @overload
    def __delitem__(self, arg0: int) -> 'None': 
        pass
    @overload
    def __delitem__(self, arg0: slice) -> 'None': ...
    def __eq__(self, arg0: StdVectorBaseData) -> 'bool': ...
    @overload
    def __getitem__(self, s: slice) -> 'StdVectorBaseData': 
        pass
    @overload
    def __getitem__(self, arg0: int) -> 'Sofa.Core.Data': ...
    def __iter__(self) -> 'object': ...
    def __len__(self) -> 'int': ...
    def __ne__(self, arg0: StdVectorBaseData) -> 'bool': ...
    def __repr__(self) -> 'str': 
        """
Return the canonical string representation of this list.
"""
    @overload
    def __setitem__(self, arg0: int, arg1: Sofa.Core.Data) -> 'None': 
        pass
    @overload
    def __setitem__(self, arg0: slice, arg1: StdVectorBaseData) -> 'None': ...
    def append(self, x: Sofa.Core.Data) -> 'None': 
        """
Add an item to the end of the list
"""
    def count(self, x: Sofa.Core.Data) -> 'int': 
        """
Return the number of times ``x`` appears in the list
"""
    def extend(self, L: StdVectorBaseData) -> 'None': 
        """
Extend the list by appending all the items in the given list
"""
    def insert(self, i: int, x: Sofa.Core.Data) -> 'None': 
        """
Insert an item at a given position.
"""
    @overload
    def pop(self, i: int) -> 'Sofa.Core.Data': 
        pass
    @overload
    def pop(self) -> 'Sofa.Core.Data': ...
    def remove(self, x: Sofa.Core.Data) -> 'None': 
        """
Remove the first item from the list whose value is x. It is an error if there is no such item.
"""
    pass
class Vectordouble(StdVectordouble):
    def __bool__(self) -> 'bool': 
        """
Check whether the list is nonempty
"""
    def __contains__(self, x: float) -> 'bool': 
        """
Return true the container contains ``x``
"""
    @overload
    def __delitem__(self, arg0: slice) -> 'None': 
        pass
    @overload
    def __delitem__(self, arg0: int) -> 'None': ...
    def __eq__(self, arg0: StdVectordouble) -> 'bool': ...
    @overload
    def __getitem__(self, s: slice) -> 'StdVectordouble': 
        pass
    @overload
    def __getitem__(self, arg0: int) -> 'float': ...
    def __iter__(self) -> 'object': ...
    def __len__(self) -> 'int': ...
    def __ne__(self, arg0: StdVectordouble) -> 'bool': ...
    def __repr__(self) -> 'str': 
        """
Return the canonical string representation of this list.
"""
    @overload
    def __setitem__(self, arg0: int, arg1: float) -> 'None': 
        pass
    @overload
    def __setitem__(self, arg0: slice, arg1: StdVectordouble) -> 'None': ...
    def append(self, x: float) -> 'None': 
        """
Add an item to the end of the list
"""
    def count(self, x: float) -> 'int': 
        """
Return the number of times ``x`` appears in the list
"""
    def extend(self, L: StdVectordouble) -> 'None': 
        """
Extend the list by appending all the items in the given list
"""
    def insert(self, i: int, x: float) -> 'None': 
        """
Insert an item at a given position.
"""
    @overload
    def pop(self) -> 'float': 
        pass
    @overload
    def pop(self, i: int) -> 'float': ...
    def remove(self, x: float) -> 'None': 
        """
Remove the first item from the list whose value is x. It is an error if there is no such item.
"""
    pass
class Vectorfloat(StdVectorfloat):
    def __bool__(self) -> 'bool': 
        """
Check whether the list is nonempty
"""
    def __contains__(self, x: float) -> 'bool': 
        """
Return true the container contains ``x``
"""
    @overload
    def __delitem__(self, arg0: int) -> 'None': 
        pass
    @overload
    def __delitem__(self, arg0: slice) -> 'None': ...
    def __eq__(self, arg0: StdVectorfloat) -> 'bool': ...
    @overload
    def __getitem__(self, s: slice) -> 'StdVectorfloat': 
        pass
    @overload
    def __getitem__(self, arg0: int) -> 'float': ...
    def __iter__(self) -> 'object': ...
    def __len__(self) -> 'int': ...
    def __ne__(self, arg0: StdVectorfloat) -> 'bool': ...
    def __repr__(self) -> 'str': 
        """
Return the canonical string representation of this list.
"""
    @overload
    def __setitem__(self, arg0: slice, arg1: StdVectorfloat) -> 'None': 
        pass
    @overload
    def __setitem__(self, arg0: int, arg1: float) -> 'None': ...
    def append(self, x: float) -> 'None': 
        """
Add an item to the end of the list
"""
    def count(self, x: float) -> 'int': 
        """
Return the number of times ``x`` appears in the list
"""
    def extend(self, L: StdVectorfloat) -> 'None': 
        """
Extend the list by appending all the items in the given list
"""
    def insert(self, i: int, x: float) -> 'None': 
        """
Insert an item at a given position.
"""
    @overload
    def pop(self) -> 'float': 
        pass
    @overload
    def pop(self, i: int) -> 'float': ...
    def remove(self, x: float) -> 'None': 
        """
Remove the first item from the list whose value is x. It is an error if there is no such item.
"""
    pass
class Vectorint(StdVectorint):
    def __bool__(self) -> 'bool': 
        """
Check whether the list is nonempty
"""
    def __contains__(self, x: int) -> 'bool': 
        """
Return true the container contains ``x``
"""
    @overload
    def __delitem__(self, arg0: int) -> 'None': 
        pass
    @overload
    def __delitem__(self, arg0: slice) -> 'None': ...
    def __eq__(self, arg0: StdVectorint) -> 'bool': ...
    @overload
    def __getitem__(self, arg0: int) -> 'int': 
        pass
    @overload
    def __getitem__(self, s: slice) -> 'StdVectorint': ...
    def __iter__(self) -> 'object': ...
    def __len__(self) -> 'int': ...
    def __ne__(self, arg0: StdVectorint) -> 'bool': ...
    def __repr__(self) -> 'str': 
        """
Return the canonical string representation of this list.
"""
    @overload
    def __setitem__(self, arg0: slice, arg1: StdVectorint) -> 'None': 
        pass
    @overload
    def __setitem__(self, arg0: int, arg1: int) -> 'None': ...
    def append(self, x: int) -> 'None': 
        """
Add an item to the end of the list
"""
    def count(self, x: int) -> 'int': 
        """
Return the number of times ``x`` appears in the list
"""
    def extend(self, L: StdVectorint) -> 'None': 
        """
Extend the list by appending all the items in the given list
"""
    def insert(self, i: int, x: int) -> 'None': 
        """
Insert an item at a given position.
"""
    @overload
    def pop(self, i: int) -> 'int': 
        pass
    @overload
    def pop(self) -> 'int': ...
    def remove(self, x: int) -> 'None': 
        """
Remove the first item from the list whose value is x. It is an error if there is no such item.
"""
    pass
class Vectorlong(StdVectorlong):
    def __bool__(self) -> 'bool': 
        """
Check whether the list is nonempty
"""
    def __contains__(self, x: int) -> 'bool': 
        """
Return true the container contains ``x``
"""
    @overload
    def __delitem__(self, arg0: int) -> 'None': 
        pass
    @overload
    def __delitem__(self, arg0: slice) -> 'None': ...
    def __eq__(self, arg0: StdVectorlong) -> 'bool': ...
    @overload
    def __getitem__(self, s: slice) -> 'StdVectorlong': 
        pass
    @overload
    def __getitem__(self, arg0: int) -> 'int': ...
    def __iter__(self) -> 'object': ...
    def __len__(self) -> 'int': ...
    def __ne__(self, arg0: StdVectorlong) -> 'bool': ...
    def __repr__(self) -> 'str': 
        """
Return the canonical string representation of this list.
"""
    @overload
    def __setitem__(self, arg0: int, arg1: int) -> 'None': 
        pass
    @overload
    def __setitem__(self, arg0: slice, arg1: StdVectorlong) -> 'None': ...
    def append(self, x: int) -> 'None': 
        """
Add an item to the end of the list
"""
    def count(self, x: int) -> 'int': 
        """
Return the number of times ``x`` appears in the list
"""
    def extend(self, L: StdVectorlong) -> 'None': 
        """
Extend the list by appending all the items in the given list
"""
    def insert(self, i: int, x: int) -> 'None': 
        """
Insert an item at a given position.
"""
    @overload
    def pop(self, i: int) -> 'int': 
        pass
    @overload
    def pop(self) -> 'int': ...
    def remove(self, x: int) -> 'None': 
        """
Remove the first item from the list whose value is x. It is an error if there is no such item.
"""
    pass
class Vectoruint(StdVectoruint):
    def __bool__(self) -> 'bool': 
        """
Check whether the list is nonempty
"""
    def __contains__(self, x: int) -> 'bool': 
        """
Return true the container contains ``x``
"""
    @overload
    def __delitem__(self, arg0: int) -> 'None': 
        pass
    @overload
    def __delitem__(self, arg0: slice) -> 'None': ...
    def __eq__(self, arg0: StdVectoruint) -> 'bool': ...
    @overload
    def __getitem__(self, arg0: int) -> 'int': 
        pass
    @overload
    def __getitem__(self, s: slice) -> 'StdVectoruint': ...
    def __iter__(self) -> 'object': ...
    def __len__(self) -> 'int': ...
    def __ne__(self, arg0: StdVectoruint) -> 'bool': ...
    def __repr__(self) -> 'str': 
        """
Return the canonical string representation of this list.
"""
    @overload
    def __setitem__(self, arg0: slice, arg1: StdVectoruint) -> 'None': 
        pass
    @overload
    def __setitem__(self, arg0: int, arg1: int) -> 'None': ...
    def append(self, x: int) -> 'None': 
        """
Add an item to the end of the list
"""
    def count(self, x: int) -> 'int': 
        """
Return the number of times ``x`` appears in the list
"""
    def extend(self, L: StdVectoruint) -> 'None': 
        """
Extend the list by appending all the items in the given list
"""
    def insert(self, i: int, x: int) -> 'None': 
        """
Insert an item at a given position.
"""
    @overload
    def pop(self, i: int) -> 'int': 
        pass
    @overload
    def pop(self) -> 'int': ...
    def remove(self, x: int) -> 'None': 
        """
Remove the first item from the list whose value is x. It is an error if there is no such item.
"""
    pass
class Vectorulong(StdVectorulong):
    def __bool__(self) -> 'bool': 
        """
Check whether the list is nonempty
"""
    def __contains__(self, x: int) -> 'bool': 
        """
Return true the container contains ``x``
"""
    @overload
    def __delitem__(self, arg0: int) -> 'None': 
        pass
    @overload
    def __delitem__(self, arg0: slice) -> 'None': ...
    def __eq__(self, arg0: StdVectorulong) -> 'bool': ...
    @overload
    def __getitem__(self, arg0: int) -> 'int': 
        pass
    @overload
    def __getitem__(self, s: slice) -> 'StdVectorulong': ...
    def __iter__(self) -> 'object': ...
    def __len__(self) -> 'int': ...
    def __ne__(self, arg0: StdVectorulong) -> 'bool': ...
    def __repr__(self) -> 'str': 
        """
Return the canonical string representation of this list.
"""
    @overload
    def __setitem__(self, arg0: int, arg1: int) -> 'None': 
        pass
    @overload
    def __setitem__(self, arg0: slice, arg1: StdVectorulong) -> 'None': ...
    def append(self, x: int) -> 'None': 
        """
Add an item to the end of the list
"""
    def count(self, x: int) -> 'int': 
        """
Return the number of times ``x`` appears in the list
"""
    def extend(self, L: StdVectorulong) -> 'None': 
        """
Extend the list by appending all the items in the given list
"""
    def insert(self, i: int, x: int) -> 'None': 
        """
Insert an item at a given position.
"""
    @overload
    def pop(self, i: int) -> 'int': 
        pass
    @overload
    def pop(self) -> 'int': ...
    def remove(self, x: int) -> 'None': 
        """
Remove the first item from the list whose value is x. It is an error if there is no such item.
"""
    pass
def msg_deprecated(*args) -> None:
    pass
def msg_error(*args) -> None:
    pass
def msg_fatal(*args) -> None:
    pass
def msg_info(*args) -> None:
    """
Emit an info message from python.
            The 'info' messages are emitted *only* when the object.printLog is
            set to True.

            Examples:
                msg_info("something bad happens")
                msg_info(sofaObject, "something bad happens")
                msg_info(sofaNode, "something bad happens")
                msg_info(emitting_file, emitting_loc, "something bad happens")
                msg_info(sofaObject, "something bad happens", emitting_file, emitting_loc)
                msg_info(sofaNode, "something bad happens", emitting_file, emitting_loc)

            Notes:
                The way the message are printed depends on the application.
                Messages can be routed the console, log files, GUI or ignored.
            
"""
def msg_warning(*args) -> None:
    pass
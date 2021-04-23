"""
           Utility functions
           -----------------------
           The 'info' messages are emitted *only* when the object.printLog is
           set to True.

           Examples:

            .. code-block:: python

               msg_info("something bad happens")
               msg_info(sofaObject, "something bad happens")
               msg_info(sofaNode, "something bad happens")
               msg_info(emitting_file, emitting_loc, "something bad happens")
               msg_info(sofaObject, "something bad happens", emitting_file, emitting_loc)
               msg_info(sofaNode, "something bad happens", emitting_file, emitting_loc)

           Notes:
               The way the message are printed depends on the application.
               Messages can be routed the console, log files, GUI or ignored.

           .. autosummary::
               Sofa.Helper.msg_info
               Sofa.Helper.msg_warning
               Sofa.Helper.msg_error
               Sofa.Helper.msg_deprecated
               Sofa.Helper.msg_fatal
       """
import Sofa.Helper
import typing

__all__ = [
    "MessageHandler",
    "StdVectorBaseData",
    "StdVectordouble",
    "StdVectorfloat",
    "StdVectorint",
    "StdVectorlong",
    "StdVectoruint",
    "StdVectorulong",
    "System",
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


class MessageHandler():
    def __enter__(self) -> MessageHandler: ...
    def __exit__(self, arg0: object, arg1: object, arg2: object) -> None: ...
    def __init__(self) -> None: ...
    @staticmethod
    def process(*args, **kwargs) -> typing.Any: ...
    pass
class StdVectorBaseData():
    def __bool__(self) -> bool: 
        """
        Check whether the list is nonempty
        """
    @staticmethod
    def __contains__(*args, **kwargs) -> typing.Any: 
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None: 
        """
        Delete the list elements at index ``i``

        Delete list elements using a slice object
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None: ...
    def __eq__(self, arg0: StdVectorBaseData) -> bool: ...
    @staticmethod
    @typing.overload
    def __getitem__(*args, **kwargs) -> typing.Any: 
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, s: slice) -> StdVectorBaseData: ...
    @typing.overload
    def __init__(self) -> None: 
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: StdVectorBaseData) -> None: ...
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None: ...
    def __iter__(self) -> typing.Iterator: ...
    def __len__(self) -> int: ...
    def __ne__(self, arg0: StdVectorBaseData) -> bool: ...
    def __repr__(self) -> str: 
        """
        Return the canonical string representation of this list.
        """
    @staticmethod
    @typing.overload
    def __setitem__(*args, **kwargs) -> typing.Any: 
        """
        Assign list elements using a slice object
        """
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: StdVectorBaseData) -> None: ...
    @staticmethod
    def append(*args, **kwargs) -> typing.Any: 
        """
        Add an item to the end of the list
        """
    def clear(self) -> None: 
        """
        Clear the contents
        """
    @staticmethod
    def count(*args, **kwargs) -> typing.Any: 
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: StdVectorBaseData) -> None: 
        """
        Extend the list by appending all the items in the given list

        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None: ...
    @staticmethod
    def insert(*args, **kwargs) -> typing.Any: 
        """
        Insert an item at a given position.
        """
    @staticmethod
    def pop(*args, **kwargs) -> typing.Any: 
        """
        Remove and return the last item

        Remove and return the item at index ``i``
        """
    @staticmethod
    def remove(*args, **kwargs) -> typing.Any: 
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
    __hash__ = None
    pass
class StdVectordouble():
    def __bool__(self) -> bool: 
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: float) -> bool: 
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None: 
        """
        Delete the list elements at index ``i``

        Delete list elements using a slice object
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None: ...
    def __eq__(self, arg0: StdVectordouble) -> bool: ...
    @typing.overload
    def __getitem__(self, arg0: int) -> float: 
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, s: slice) -> StdVectordouble: ...
    @typing.overload
    def __init__(self) -> None: 
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: StdVectordouble) -> None: ...
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None: ...
    def __iter__(self) -> typing.Iterator: ...
    def __len__(self) -> int: ...
    def __ne__(self, arg0: StdVectordouble) -> bool: ...
    def __repr__(self) -> str: 
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: float) -> None: 
        """
        Assign list elements using a slice object
        """
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: StdVectordouble) -> None: ...
    def append(self, x: float) -> None: 
        """
        Add an item to the end of the list
        """
    def clear(self) -> None: 
        """
        Clear the contents
        """
    def count(self, x: float) -> int: 
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: StdVectordouble) -> None: 
        """
        Extend the list by appending all the items in the given list

        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None: ...
    def insert(self, i: int, x: float) -> None: 
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> float: 
        """
        Remove and return the last item

        Remove and return the item at index ``i``
        """
    @typing.overload
    def pop(self, i: int) -> float: ...
    def remove(self, x: float) -> None: 
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
    __hash__ = None
    pass
class StdVectorfloat():
    def __bool__(self) -> bool: 
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: float) -> bool: 
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None: 
        """
        Delete the list elements at index ``i``

        Delete list elements using a slice object
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None: ...
    def __eq__(self, arg0: StdVectorfloat) -> bool: ...
    @typing.overload
    def __getitem__(self, arg0: int) -> float: 
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, s: slice) -> StdVectorfloat: ...
    @typing.overload
    def __init__(self) -> None: 
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: StdVectorfloat) -> None: ...
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None: ...
    def __iter__(self) -> typing.Iterator: ...
    def __len__(self) -> int: ...
    def __ne__(self, arg0: StdVectorfloat) -> bool: ...
    def __repr__(self) -> str: 
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: float) -> None: 
        """
        Assign list elements using a slice object
        """
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: StdVectorfloat) -> None: ...
    def append(self, x: float) -> None: 
        """
        Add an item to the end of the list
        """
    def clear(self) -> None: 
        """
        Clear the contents
        """
    def count(self, x: float) -> int: 
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: StdVectorfloat) -> None: 
        """
        Extend the list by appending all the items in the given list

        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None: ...
    def insert(self, i: int, x: float) -> None: 
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> float: 
        """
        Remove and return the last item

        Remove and return the item at index ``i``
        """
    @typing.overload
    def pop(self, i: int) -> float: ...
    def remove(self, x: float) -> None: 
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
    __hash__ = None
    pass
class StdVectorint():
    def __bool__(self) -> bool: 
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: int) -> bool: 
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None: 
        """
        Delete the list elements at index ``i``

        Delete list elements using a slice object
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None: ...
    def __eq__(self, arg0: StdVectorint) -> bool: ...
    @typing.overload
    def __getitem__(self, arg0: int) -> int: 
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, s: slice) -> StdVectorint: ...
    @typing.overload
    def __init__(self) -> None: 
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: StdVectorint) -> None: ...
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None: ...
    def __iter__(self) -> typing.Iterator: ...
    def __len__(self) -> int: ...
    def __ne__(self, arg0: StdVectorint) -> bool: ...
    def __repr__(self) -> str: 
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: int) -> None: 
        """
        Assign list elements using a slice object
        """
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: StdVectorint) -> None: ...
    def append(self, x: int) -> None: 
        """
        Add an item to the end of the list
        """
    def clear(self) -> None: 
        """
        Clear the contents
        """
    def count(self, x: int) -> int: 
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: StdVectorint) -> None: 
        """
        Extend the list by appending all the items in the given list

        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None: ...
    def insert(self, i: int, x: int) -> None: 
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> int: 
        """
        Remove and return the last item

        Remove and return the item at index ``i``
        """
    @typing.overload
    def pop(self, i: int) -> int: ...
    def remove(self, x: int) -> None: 
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
    __hash__ = None
    pass
class StdVectorlong():
    def __bool__(self) -> bool: 
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: int) -> bool: 
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None: 
        """
        Delete the list elements at index ``i``

        Delete list elements using a slice object
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None: ...
    def __eq__(self, arg0: StdVectorlong) -> bool: ...
    @typing.overload
    def __getitem__(self, arg0: int) -> int: 
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, s: slice) -> StdVectorlong: ...
    @typing.overload
    def __init__(self) -> None: 
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: StdVectorlong) -> None: ...
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None: ...
    def __iter__(self) -> typing.Iterator: ...
    def __len__(self) -> int: ...
    def __ne__(self, arg0: StdVectorlong) -> bool: ...
    def __repr__(self) -> str: 
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: int) -> None: 
        """
        Assign list elements using a slice object
        """
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: StdVectorlong) -> None: ...
    def append(self, x: int) -> None: 
        """
        Add an item to the end of the list
        """
    def clear(self) -> None: 
        """
        Clear the contents
        """
    def count(self, x: int) -> int: 
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: StdVectorlong) -> None: 
        """
        Extend the list by appending all the items in the given list

        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None: ...
    def insert(self, i: int, x: int) -> None: 
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> int: 
        """
        Remove and return the last item

        Remove and return the item at index ``i``
        """
    @typing.overload
    def pop(self, i: int) -> int: ...
    def remove(self, x: int) -> None: 
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
    __hash__ = None
    pass
class StdVectoruint():
    def __bool__(self) -> bool: 
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: int) -> bool: 
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None: 
        """
        Delete the list elements at index ``i``

        Delete list elements using a slice object
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None: ...
    def __eq__(self, arg0: StdVectoruint) -> bool: ...
    @typing.overload
    def __getitem__(self, arg0: int) -> int: 
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, s: slice) -> StdVectoruint: ...
    @typing.overload
    def __init__(self) -> None: 
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: StdVectoruint) -> None: ...
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None: ...
    def __iter__(self) -> typing.Iterator: ...
    def __len__(self) -> int: ...
    def __ne__(self, arg0: StdVectoruint) -> bool: ...
    def __repr__(self) -> str: 
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: int) -> None: 
        """
        Assign list elements using a slice object
        """
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: StdVectoruint) -> None: ...
    def append(self, x: int) -> None: 
        """
        Add an item to the end of the list
        """
    def clear(self) -> None: 
        """
        Clear the contents
        """
    def count(self, x: int) -> int: 
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: StdVectoruint) -> None: 
        """
        Extend the list by appending all the items in the given list

        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None: ...
    def insert(self, i: int, x: int) -> None: 
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> int: 
        """
        Remove and return the last item

        Remove and return the item at index ``i``
        """
    @typing.overload
    def pop(self, i: int) -> int: ...
    def remove(self, x: int) -> None: 
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
    __hash__ = None
    pass
class StdVectorulong():
    def __bool__(self) -> bool: 
        """
        Check whether the list is nonempty
        """
    def __contains__(self, x: int) -> bool: 
        """
        Return true the container contains ``x``
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None: 
        """
        Delete the list elements at index ``i``

        Delete list elements using a slice object
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None: ...
    def __eq__(self, arg0: StdVectorulong) -> bool: ...
    @typing.overload
    def __getitem__(self, arg0: int) -> int: 
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, s: slice) -> StdVectorulong: ...
    @typing.overload
    def __init__(self) -> None: 
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: StdVectorulong) -> None: ...
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None: ...
    def __iter__(self) -> typing.Iterator: ...
    def __len__(self) -> int: ...
    def __ne__(self, arg0: StdVectorulong) -> bool: ...
    def __repr__(self) -> str: 
        """
        Return the canonical string representation of this list.
        """
    @typing.overload
    def __setitem__(self, arg0: int, arg1: int) -> None: 
        """
        Assign list elements using a slice object
        """
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: StdVectorulong) -> None: ...
    def append(self, x: int) -> None: 
        """
        Add an item to the end of the list
        """
    def clear(self) -> None: 
        """
        Clear the contents
        """
    def count(self, x: int) -> int: 
        """
        Return the number of times ``x`` appears in the list
        """
    @typing.overload
    def extend(self, L: StdVectorulong) -> None: 
        """
        Extend the list by appending all the items in the given list

        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None: ...
    def insert(self, i: int, x: int) -> None: 
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> int: 
        """
        Remove and return the last item

        Remove and return the item at index ``i``
        """
    @typing.overload
    def pop(self, i: int) -> int: ...
    def remove(self, x: int) -> None: 
        """
        Remove the first item from the list whose value is x. It is an error if there is no such item.
        """
    __hash__ = None
    pass
class VectorBaseData(StdVectorBaseData):
    pass
class Vectordouble(StdVectordouble):
    pass
class Vectorfloat(StdVectorfloat):
    pass
class Vectorint(StdVectorint):
    pass
class Vectorlong(StdVectorlong):
    pass
class Vectoruint(StdVectoruint):
    pass
class Vectorulong(StdVectorulong):
    pass
def msg_deprecated(*args) -> None:
    """
    Emit a deprecated message from python.
    """
def msg_error(*args) -> None:
    """
    Emit an error message from python.
    """
def msg_fatal(*args) -> None:
    """
    Emit a fatal error message from python.
    """
def msg_info(*args) -> None:
    """
    Emit an info message from python.
    """
def msg_warning(*args) -> None:
    """
    Emit a warning message from python.
    """

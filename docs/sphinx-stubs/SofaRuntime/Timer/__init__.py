"""
           Timer
           -----------------------
           Advanced timer, meant to gather precise statistics for results in published papers.
           Not so advanced for now, but it will be...
       """
import SofaRuntime.Timer
from typing import *
from typing import Iterable as iterable
from typing import Iterator as iterator
from numpy import float64
_Shape = Tuple[int, ...]
__all__  = [
"begin",
"clear",
"end",
"getInterval",
"getRecords",
"isEnabled",
"setEnabled",
"setInterval",
"setOutputType",
"stepBegin",
"stepEnd"
]
def begin(id: str) -> None:
    pass
def clear() -> None:
    pass
def end(id: str) -> None:
    pass
def getInterval(id: object) -> int:
    pass
def getRecords(id: str) -> dict:
    pass
def isEnabled(id: str) -> None:
    pass
def setEnabled(name: str, enabled: bool) -> None:
    pass
def setInterval(id: object, interval: int) -> None:
    pass
def setOutputType(id: object, newOutputType: str) -> None:
    pass
def stepBegin(id: str) -> None:
    pass
def stepEnd(id: str) -> None:
    pass
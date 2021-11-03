"""
           Timer
           -----------------------

           Advanced timer, meant to gather precise statistics for results in published papers.
           Not so advanced for now, but it will be...
       """
import SofaRuntime.SofaRuntime.Timer
import typing

__all__ = [
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
    """
    Clears the AdvancedTimer.
    """
def end(id: str) -> None:
    pass
def getInterval(id: str) -> None:
    """
    Returns the Timer's interval.
    """
def getRecords(id: str) -> dict:
    pass
def isEnabled(id: str) -> None:
    """
    Returns True if the given id is enabled, False otherwise.
    """
def setEnabled(name: str, enabled: bool) -> None:
    """
    Enables or disables the given timer.
    """
def setInterval(id: str, interval: int) -> None:
    """
    Sets the interval for the given timer.
    """
def setOutputType(id: str, newOutputType: str) -> None:
    """
    Set the outputType for the given AdvancedTimer.

    @param id string id of the timer
    @param type string output type name (example : "json")
    """
def stepBegin(id: str) -> None:
    pass
def stepEnd(id: str) -> None:
    pass

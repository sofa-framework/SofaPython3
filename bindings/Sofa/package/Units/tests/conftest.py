"""
Makes the `Units` package importable by the test files in this folder.

The modules under test (`Core`, `Definitions`, `UnitSystem`) use
package-relative imports (`from .Core import *`), so they must be imported
through the `Units` package rather than as top-level modules. This folder is
`Units/tests`, so the package's parent directory is two levels up; inserting
it into sys.path lets the tests do `from Units.Core import ...`.

pytest imports conftest.py before collecting/importing the test modules in
the same directory, so doing this here is enough.
"""
import os
import sys

_PACKAGE_PARENT = os.path.abspath(
    os.path.join(os.path.dirname(__file__), os.pardir, os.pardir)
)

if _PACKAGE_PARENT not in sys.path:
    sys.path.insert(0, _PACKAGE_PARENT)

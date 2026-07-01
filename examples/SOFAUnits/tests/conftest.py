"""
Allows the test files in this folder to `import units` and
`import SimulationParameters` even though this folder is a sub-folder of
the project root (where those two modules actually live).

pytest imports conftest.py before collecting/importing the test modules in
the same directory, so inserting the parent folder into sys.path here is
enough - the test files themselves don't need to know anything about the
project layout.
"""
import os
import sys

_PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir))

if _PROJECT_ROOT not in sys.path:
    sys.path.insert(0, _PROJECT_ROOT)

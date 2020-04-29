"""
Meshing module
----------------
Meshing Functionality using python3-bindings Gmsh and CGAL.

Submodules:
  .. autosummary::
    :toctree: _autosummary

    splib.meshing.gmsh_sofa




"""
__all__=["gmsh_sofa"] #, "CGAL"]

from .gmsh_sofa import meshFromParametricGeometry


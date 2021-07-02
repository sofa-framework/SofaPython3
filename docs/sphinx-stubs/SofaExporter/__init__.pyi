"""
              Binding for the SofaExporter plugin
              -----------------------------------

              Provides python bindings for the SofaExporter module

              Example of use:

              .. code-block:: python

                import SofaExporter

              .. autosummary::
                  :toctree: _autosummary/_autosummary

                  SofaExporter.STLExporter

              """
import SofaExporter
import typing
import Sofa.Core

__all__ = [
    "STLExporter"
]


class STLExporter(Sofa.Core.Object, Sofa.Core.Base):
    def write(self) -> bool: 
        """
                Exports an STL file
                ---------------------------------------

                Will export a binary or ascii file depending on the binary flag of STLExporter
                Will auto-number the exported files

                Example of use:
                  .. code-block:: python

                     import Sofa
                     import SofaExporter

                     # Create a new node
                     n = Sofa.Core.Node("root"")

                     # Add STLExporter
                     n.addObject("STLExporter", name="exporter", ...)

                     # writes down the stl file
                     n.exporter.write()

                
        """
    pass

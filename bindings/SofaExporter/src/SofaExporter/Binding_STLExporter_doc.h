#pragma once

namespace sofapython3::doc::SofaExporter::STLExporter::write
{

static auto docstring =
        R"(
        Exports a STL file
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

        )";
}

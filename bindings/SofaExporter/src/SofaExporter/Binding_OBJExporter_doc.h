/******************************************************************************
*                              SofaPython3 plugin                             *
*                  (c) 2021 CNRS, University of Lille, INRIA                  *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#pragma once

namespace sofapython3::doc::SofaExporter::OBJExporter::write {

static auto docstring =
        R"(
        Exports an OBJ file
        ---------------------------------------

        Will export a binary or ascii file depending on the binary flag of OBJExporter
        Will auto-number the exported files

        Example of use:
          .. code-block:: python

             import Sofa
             import SofaExporter

             # Create a new node
             n = Sofa.Core.Node("root"")

             # Add STLExporter
             n.addObject("OBJExporter", name="exporter", ...)

             # writes down the stl file
             n.exporter.write()

        )";

}

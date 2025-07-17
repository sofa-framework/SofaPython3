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

#include <pybind11/pybind11.h>

#include <SofaPython3/Sofa/Helper/Binding_Utils.h>
#include <SofaPython3/PythonFactory.h>

#include <sofa/helper/Utils.h>

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

namespace sofapython3
{
    
void moduleAddUtils(py::module &m) {
    py::class_<sofa::helper::Utils> utils(m, "Utils");
    utils.doc() = "Utility class with convenient functions.";

    const auto GetSofaUserLocalDirectoryDoc = R"doc(
        Get the directory where is stored the sofa configuration.
    )doc";
    utils.def_static("GetSofaUserLocalDirectory", &sofa::helper::Utils::getSofaUserLocalDirectory, GetSofaUserLocalDirectoryDoc);
    
    const auto GetSofaDataDirectoryDoc = R"doc(
        Get the directory where is stored the sofa output data such as screenshots.
    )doc";
    utils.def_static("GetSofaDataDirectory", &sofa::helper::Utils::getSofaDataDirectory, GetSofaDataDirectoryDoc);
}


}

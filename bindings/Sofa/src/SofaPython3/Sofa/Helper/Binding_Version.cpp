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

#include <SofaPython3/PythonFactory.h>
#include <SofaPython3/Sofa/Helper/Binding_Version.h>

#include <iomanip>
#include <sofa/version.h>


/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

namespace sofapython3
{

void moduleAddVersion(py::module &m)
{
    m.def("GetVersion",
        []()
        {
            static const std::string sofaVersion = []() {
                std::stringstream version;
                constexpr auto major = SOFA_VERSION / 10000;
                constexpr auto minor = SOFA_VERSION / 100 % 100;
                version << 'v'
                    << std::setfill('0') << std::setw(2) << major
                    << "."
                    << std::setfill('0') << std::setw(2) << minor;
                return version.str();
            }();
            return sofaVersion;
        },
        "Returns the version of SOFA as a string in the format 'vMM.mm', where MM is the major version and mm is the minor version.");
}


}

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

#include "Binding_Vector.h"

#include <sofa/type/vector.h>
#include <sofa/core/objectmodel/BaseData.h>

#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

namespace py { using namespace pybind11; }


/**
 * To add sofa::type::vector types, follow the 2 steps below
 */

// ------------------------------------
// STEP 1: Make the std vector type opaque
// ------------------------------------
PYBIND11_MAKE_OPAQUE(std::vector<int>);
PYBIND11_MAKE_OPAQUE(std::vector<long>);
PYBIND11_MAKE_OPAQUE(std::vector<unsigned int>);
PYBIND11_MAKE_OPAQUE(std::vector<unsigned long>);
PYBIND11_MAKE_OPAQUE(std::vector<float>);
PYBIND11_MAKE_OPAQUE(std::vector<double>);
PYBIND11_MAKE_OPAQUE(std::vector<sofa::core::objectmodel::BaseData*>);

template<typename T>
void declareVector(py::module &m, const std::string &typestr) {
    std::string std_pyclass_name = std::string("StdVector") + typestr;
    auto v = py::bind_vector<std::vector<T>>(m, std_pyclass_name);

    std::string pyclass_name = std::string("Vector") + typestr;

    py::class_<sofa::type::vector<T>> (m, pyclass_name.c_str(), v);
}

namespace sofapython3 {

void moduleAddVector(py::module &m) {
// ------------------------------------
// STEP 2: Declare the vector type to python
// ------------------------------------
    declareVector<int>(m, "int");
    declareVector<long>(m, "long");
    declareVector<unsigned int>(m, "uint");
    declareVector<unsigned long>(m, "ulong");
    declareVector<float>(m, "float");
    declareVector<double>(m, "double");
    declareVector<sofa::core::objectmodel::BaseData*>(m, "BaseData");
}

} /// namespace sofapython3
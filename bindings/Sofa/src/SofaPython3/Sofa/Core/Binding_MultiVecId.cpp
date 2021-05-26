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

#include <SofaPython3/Sofa/Core/Binding_MultiVecId.h>
#include <sofa/core/MultiVecId.h>
#include <sofa/core/BaseState.h>

namespace py { using namespace pybind11; }
using namespace sofa::core;

namespace sofapython3 {

template <VecType vtype, VecAccess vaccess>
void bindMultiVecId(py::module& m, const std::string & name) {
    using MultiVecId = TMultiVecId<vtype, vaccess>;
    py::class_<MultiVecId> c (m, name.c_str());

    // Constructors
    c.def(py::init());

    // Read-only can be constructed from both read-only and read-write multi vectors
    if constexpr (vaccess == V_READ) {

        c.def(py::init<const TVecId<vtype, V_WRITE> &>());
        py::implicitly_convertible<TVecId<vtype, V_WRITE>, TMultiVecId<vtype, vaccess>>();

        c.def(py::init<const TMultiVecId<vtype, V_WRITE> &>());
        py::implicitly_convertible<TMultiVecId<vtype, V_WRITE>, TMultiVecId<vtype, vaccess>>();

        if constexpr(vtype != V_ALL) {
            c.def(py::init<const TMultiVecId<V_ALL, V_WRITE> &>());
            py::implicitly_convertible<TMultiVecId<V_ALL, V_WRITE>, TMultiVecId<vtype, vaccess>>();
        } else {
            c.def(py::init<const TMultiVecId<V_COORD, V_WRITE> &>());
            py::implicitly_convertible<TMultiVecId<V_COORD, V_WRITE>, TMultiVecId<vtype, vaccess>>();

            c.def(py::init<const TMultiVecId<V_DERIV, V_WRITE> &>());
            py::implicitly_convertible<TMultiVecId<V_DERIV, V_WRITE>, TMultiVecId<vtype, vaccess>>();

            c.def(py::init<const TMultiVecId<V_MATDERIV, V_WRITE> &>());
            py::implicitly_convertible<TMultiVecId<V_MATDERIV, V_WRITE>, TMultiVecId<vtype, vaccess>>();

            c.def(py::init<const TVecId<V_COORD, V_WRITE> &>());
            py::implicitly_convertible<TVecId<V_COORD, V_WRITE>, TMultiVecId<vtype, vaccess>>();

            c.def(py::init<const TVecId<V_DERIV, V_WRITE> &>());
            py::implicitly_convertible<TVecId<V_DERIV, V_WRITE>, TMultiVecId<vtype, vaccess>>();

            c.def(py::init<const TVecId<V_MATDERIV, V_WRITE> &>());
            py::implicitly_convertible<TVecId<V_MATDERIV, V_WRITE>, TMultiVecId<vtype, vaccess>>();
        }
    }

    // Construct from the same access type
    c.def(py::init<const TVecId<vtype, vaccess>&>());
    py::implicitly_convertible<TVecId<vtype, vaccess>, TMultiVecId<vtype, vaccess>>();

    c.def(py::init<const TMultiVecId<vtype, vaccess>&>());

    // Construct from a ALL vector type
    if constexpr(vtype != V_ALL) {
        c.def(py::init<const TMultiVecId<V_ALL, vaccess> &>());
        py::implicitly_convertible<TMultiVecId<V_ALL, vaccess>, TMultiVecId<vtype, vaccess>>();
    } else {
        c.def(py::init<const TVecId<V_COORD, vaccess> &>());
        py::implicitly_convertible<TVecId<V_COORD, vaccess>, TMultiVecId<vtype, vaccess>>();

        c.def(py::init<const TMultiVecId<V_COORD, vaccess> &>());
        py::implicitly_convertible<TMultiVecId<V_COORD, vaccess>, TMultiVecId<vtype, vaccess>>();

        c.def(py::init<const TVecId<V_DERIV, vaccess> &>());
        py::implicitly_convertible<TVecId<V_DERIV, vaccess>, TMultiVecId<vtype, vaccess>>();

        c.def(py::init<const TMultiVecId<V_DERIV, vaccess> &>());
        py::implicitly_convertible<TMultiVecId<V_DERIV, vaccess>, TMultiVecId<vtype, vaccess>>();

        c.def(py::init<const TVecId<V_MATDERIV, vaccess> &>());
        py::implicitly_convertible<TVecId<V_MATDERIV, vaccess>, TMultiVecId<vtype, vaccess>>();

        c.def(py::init<const TMultiVecId<V_MATDERIV, vaccess> &>());
        py::implicitly_convertible<TMultiVecId<V_MATDERIV, vaccess>, TMultiVecId<vtype, vaccess>>();
    }

    // Public methods/properties
    c.def_property_readonly("defaultId", &MultiVecId::getDefaultId);
    c.def("getDefaultId", &MultiVecId::getDefaultId);

    c.def("getName", &MultiVecId::getName);
    c.def("isNull", &MultiVecId::isNull);
    c.def("__str__", &MultiVecId::getName);
    c.def("getId", &MultiVecId::getId, py::arg("state"));

    // Static methods/properties
    c.def_static("null", &MultiVecId::null);
}

void moduleAddMultiVecId(py::module& m) {
    bindMultiVecId<V_ALL, V_READ>  (m, "ConstMultiVecId");
    bindMultiVecId<V_ALL, V_WRITE> (m, "MultiVecId");
    bindMultiVecId<V_COORD, V_READ>  (m, "ConstMultiVecCoordId");
    bindMultiVecId<V_COORD, V_WRITE> (m, "MultiVecCoordId");
    bindMultiVecId<V_DERIV, V_READ>  (m, "ConstMultiVecDerivId");
    bindMultiVecId<V_DERIV, V_WRITE> (m, "MultiVecDerivId");
    bindMultiVecId<V_MATDERIV, V_READ>  (m, "ConstMultiMatrixDerivId");
    bindMultiVecId<V_MATDERIV, V_WRITE> (m, "MultiMatrixDerivId");
}

} // namespace sofapython3

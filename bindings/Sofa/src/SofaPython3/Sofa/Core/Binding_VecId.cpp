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

#include <SofaPython3/Sofa/Core/Binding_VecId.h>
#include <sofa/core/VecId.h>

namespace py { using namespace pybind11; }
using namespace sofa::core;

namespace sofapython3 {

template <VecType vtype, VecAccess vaccess>
void bindVecId(py::module& m, const std::string & name) {

    using StandardVec = TStandardVec<vtype, vaccess>;
    py::class_<StandardVec> s (m, ("Standard"+name).c_str());
    if constexpr (vtype == VecType::V_COORD || vtype == VecType::V_ALL) {
        s.def_static("position", &StandardVec::position);
        s.def_static("restPosition", &StandardVec::restPosition);
        s.def_static("freePosition", &StandardVec::freePosition);
        s.def_static("resetPosition", &StandardVec::resetPosition);
    }
    if constexpr (vtype == VecType::V_DERIV || vtype == VecType::V_ALL) {
        s.def_static("velocity", &StandardVec::velocity);
        s.def_static("resetVelocity", &StandardVec::resetVelocity);
        s.def_static("freeVelocity", &StandardVec::freeVelocity);
        s.def_static("normal", &StandardVec::normal);
        s.def_static("force", &StandardVec::force);
        s.def_static("externalForce", &StandardVec::externalForce);
        s.def_static("dx", &StandardVec::dx);
        s.def_static("dforce", &StandardVec::dforce);
    }
    if constexpr(vtype == VecType::V_MATDERIV || vtype == VecType::V_ALL) {
        s.def_static("constraintJacobian", &StandardVec::constraintJacobian);
        s.def_static("mappingJacobian", &StandardVec::mappingJacobian);
    }

    if constexpr (vtype == VecType::V_ALL) {
        s.def_static("getFirstDynamicIndex", &StandardVec::getFirstDynamicIndex, py::arg("t"));
    }

    using VecId = TVecId<vtype, vaccess>;
    py::class_<VecId, StandardVec, BaseVecId> v (m, name.c_str());
    v.def(py::init());
    if constexpr (vtype != V_ALL) {
        py::implicitly_convertible<TVecId<vtype, vaccess>, TVecId<V_ALL, vaccess>>();
        v.def(py::init<unsigned int>());
    }
    v.def("getName", &VecId::getName);
    v.def("__str__", &VecId::getName);
}

void moduleAddVecId(py::module& m) {

    // VecType
    py::enum_<VecType> (m, "VecType")
        .value("V_ALL", VecType::V_ALL)
        .value("V_COORD", VecType::V_COORD)
        .value("V_DERIV", VecType::V_DERIV)
        .value("V_MATDERIV", VecType::V_MATDERIV)
        .export_values()
    ;

    // VecAccess
    py::enum_<VecAccess> (m, "VecAccess")
        .value("V_READ", VecAccess::V_READ)
        .value("V_WRITE", VecAccess::V_WRITE)
        .export_values()
    ;

    // BaseVecId
    py::class_<BaseVecId> (m, "BaseVecId")
        .def_property_readonly("index", &BaseVecId::getIndex)
        .def("getIndex", &BaseVecId::getIndex)
        .def_property_readonly("type", &BaseVecId::getType)
        .def("getType", &BaseVecId::getType)
    ;

    // VecId
    bindVecId<V_ALL, V_READ> (m, "ConstVecId");
    bindVecId<V_ALL, V_WRITE> (m, "VecId");
    bindVecId<V_COORD, V_READ> (m, "ConstVecCoordId");
    bindVecId<V_COORD, V_WRITE>(m, "VecCoordId");
    bindVecId<V_DERIV, V_READ> (m, "ConstVecDerivId");
    bindVecId<V_DERIV, V_WRITE>(m, "VecDerivId");
    bindVecId<V_MATDERIV, V_READ> (m, "ConstMatrixDerivId");
    bindVecId<V_MATDERIV, V_WRITE>(m, "MatrixDerivId");
}

} // namespace sofapython3

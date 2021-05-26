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

#include <SofaPython3/Sofa/Core/Binding_BaseState.h>
#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <sofa/core/BaseState.h>

#include <SofaPython3/PythonFactory.h>

namespace py { using namespace pybind11; }

namespace sofapython3 {

void moduleAddBaseState(py::module& m) {
    using namespace sofa::core;
    py::class_<BaseState, sofa::core::objectmodel::BaseObject, py_shared_ptr<BaseState>> (m, "BaseState")
    .def("getSize", &BaseState::getSize)
    .def("resize", &BaseState::getSize)
    ;

    /// register the BaseState binding in the downcasting subsystem
    PythonFactory::registerType<BaseState>([](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<BaseState*>(object));
    });
}

} // namespace sofapython3

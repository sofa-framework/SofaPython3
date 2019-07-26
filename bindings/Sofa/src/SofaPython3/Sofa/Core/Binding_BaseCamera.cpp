/*********************************************************************
Copyright 2019, Inria, CNRS, University of Lille

This file is part of runSofa2

runSofa2 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

runSofa2 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with sofaqtquick. If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/
/********************************************************************
 Contributors:
    - damien.marchal@univ-lille.fr
********************************************************************/
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/detail/init.h>

#include "Binding_Base.h"
#include "Binding_BaseObject.h"
#include "Binding_BaseCamera.h"

#include <SofaPython3/PythonDownCast.h>

namespace sofapython3
{

std::vector<double> getProjectionMatrix(BaseCamera* self)
{
    static std::vector<double> m {16};
    m.resize(16);
    self->getProjectionMatrix(m.data());
    return m;
}

std::vector<double> getOpenGLProjectionMatrix(BaseCamera* self)
{
    static std::vector<double> m {16};
    m.resize(16);
    self->getOpenGLProjectionMatrix(m.data());
    return m;
}

std::vector<double> getOpenGLModelViewMatrix(BaseCamera* self)
{
    auto s = self->p_position.getValue();
    static std::vector<double> m {16};
    m.resize(16);
    self->getOpenGLModelViewMatrix(m.data());
    return m;
}

std::vector<double> getModelViewMatrix(BaseCamera* self)
{
    static std::vector<double> m {16};
    m.resize(16);
    self->getModelViewMatrix(m.data());
    return m;
}


void moduleAddBaseCamera(py::module &m)
{
    /// register the BaseCamera binding in the pybind11 typeing sytem
    py::class_<BaseCamera,
               sofa::core::objectmodel::BaseObject,
               sofa::core::sptr<BaseCamera>> c(m, "Camera");

    /// register the BaseCamera binding in the downcasting subsystem
    PythonDownCast::registerType<BaseCamera>([](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<BaseCamera*>(object));
    });

    c.def("getProjectionMatrix", getProjectionMatrix);
    c.def("getOpenGLProjectionMatrix", getOpenGLProjectionMatrix);
    c.def("getModelViewMatrix", getModelViewMatrix);
    c.def("getOpenGLModelViewMatrix", getOpenGLModelViewMatrix);
}

} /// namespace sofapython3

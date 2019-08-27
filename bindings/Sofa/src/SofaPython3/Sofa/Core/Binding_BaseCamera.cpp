/*********************************************************************
Copyright 2019, CNRS, University of Lille, INRIA

This file is part of sofaPython3

sofaPython3 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

sofaPython3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with sofaqtquick. If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/
/********************************************************************
 Contributors:
    - damien.marchal@univ-lille.fr
    - bruno.josue.marques@inria.fr
    - eve.le-guillou@centrale.centralelille.fr
    - jean-nicolas.brunet@inria.fr
    - thierry.gaugry@inria.fr
********************************************************************/

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/detail/init.h>

#include "Binding_Base.h"
#include "Binding_BaseObject.h"
#include "Binding_BaseCamera.h"

#include <SofaPython3/PythonFactory.h>
#include "Binding_BaseCamera_doc.h"

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
               sofa::core::sptr<BaseCamera>> c(m, "Camera", sofapython3::doc::baseCamera::baseCameraClass);

    /// register the BaseCamera binding in the downcasting subsystem
    PythonFactory::registerType<BaseCamera>([](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<BaseCamera*>(object));
    });

    c.def("getProjectionMatrix", getProjectionMatrix);
    c.def("getOpenGLProjectionMatrix", getOpenGLProjectionMatrix);
    c.def("getModelViewMatrix", getModelViewMatrix);
    c.def("getOpenGLModelViewMatrix", getOpenGLModelViewMatrix);
}

} /// namespace sofapython3

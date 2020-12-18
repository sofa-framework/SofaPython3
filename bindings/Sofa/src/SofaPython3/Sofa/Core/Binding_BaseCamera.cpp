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

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaBaseVisual/BaseCamera.h>

#include <SofaPython3/Sofa/Core/Binding_BaseCamera.h>
#include <SofaPython3/Sofa/Core/Binding_BaseCamera_doc.h>

#include <SofaPython3/PythonFactory.h>

namespace py { using namespace pybind11; }
using sofa::core::objectmodel::BaseObject;
using sofa::component::visualmodel::BaseCamera;

namespace sofapython3 {

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
               py_shared_ptr<BaseCamera>> c(m, "Camera", sofapython3::doc::baseCamera::baseCameraClass);

    /// register the BaseCamera binding in the downcasting subsystem
    PythonFactory::registerType<BaseCamera>([](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<BaseCamera*>(object));
    });

    c.def("getProjectionMatrix", getProjectionMatrix);
    c.def("getOpenGLProjectionMatrix", getOpenGLProjectionMatrix);
    c.def("getModelViewMatrix", getModelViewMatrix);
    c.def("getOpenGLModelViewMatrix", getOpenGLModelViewMatrix);

    c.def("rotate", [](BaseCamera *self, py::list p){
        sofa::defaulttype::Quat vec;
        vec = sofa::defaulttype::Quat(py::cast<double>(p[0]),py::cast<double>(p[1]),py::cast<double>(p[2]),py::cast<double>(p[3]));
        self->rotate(vec);
    });

    c.def("rotateCameraAroundPoint", [](BaseCamera *self, py::list p1, py::list p2) {
         sofa::defaulttype::Quat vec1;
         sofa::defaulttype::Vec3 vec2;
         vec1 = sofa::defaulttype::Quat(py::cast<double>(p1[0]),py::cast<double>(p1[1]),py::cast<double>(p1[2]),py::cast<double>(p1[3]));
         vec2 = sofa::defaulttype::Vec3(py::cast<double>(p2[0]),py::cast<double>(p2[1]),py::cast<double>(p2[2]));
         self->rotateCameraAroundPoint(vec1, vec2);
    });

    c.def("getOrientationFromLookAt", [](BaseCamera *self, py::list p1, py::list p2) {
         sofa::defaulttype::Vec3 vec1;
         sofa::defaulttype::Vec3 vec2;
         vec1 = sofa::defaulttype::Vec3(py::cast<double>(p1[0]),py::cast<double>(p1[1]),py::cast<double>(p1[2]));
         vec2 = sofa::defaulttype::Vec3(py::cast<double>(p2[0]),py::cast<double>(p2[1]),py::cast<double>(p2[2]));
         return (self->getOrientationFromLookAt(vec1, vec2));
    });

    c.def("getPositionFromOrientation", [](BaseCamera *self, py::list p1, py::float_ p2, py::list p3) {
         sofa::defaulttype::Vec3 vec1;
         double vec2;
         sofa::defaulttype::Quat vec3;
         vec1 = sofa::defaulttype::Vec3(py::cast<double>(p1[0]),py::cast<double>(p1[1]),py::cast<double>(p1[2]));
         vec2 = py::cast<double>(p2);
         vec3 = sofa::defaulttype::Quat(py::cast<double>(p3[0]),py::cast<double>(p3[1]),py::cast<double>(p3[2]),py::cast<double>(p3[3]));
         return(self->getPositionFromOrientation(vec1, vec2, vec3));
    });

    c.def("setCameraType", [](BaseCamera *self, int p1) {
         self->setCameraType(p1);
    });

    c.def("Orthographic", [](BaseCamera *self) {
         self->setCameraType(1);
    });

    c.def("Perspective", [](BaseCamera *self) {
         self->setCameraType(0);
    });

}

} /// namespace sofapython3

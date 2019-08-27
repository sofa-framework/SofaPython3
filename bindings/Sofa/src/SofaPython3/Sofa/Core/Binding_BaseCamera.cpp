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

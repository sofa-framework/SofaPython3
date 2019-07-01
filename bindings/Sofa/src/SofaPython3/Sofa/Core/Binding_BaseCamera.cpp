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

#include <SofaPython3/Sofa/Core/PythonDownCast.h>

namespace sofapython3
{
void moduleAddBaseCamera(py::module &m) {
    py::class_<sofa::component::visualmodel::BaseCamera,
            sofa::core::sptr<sofa::component::visualmodel::BaseCamera>> c(m, "Camera");

    c.def(py::init([](BaseObject* b){
              auto camera = dynamic_cast<BaseCamera*>(b);
              if(camera)
                  return camera;
              throw std::invalid_argument(b->getClassName());
          }), ":rtype: Sofa.Core.Camera");

    c.def("getProjectionMatrix", [](BaseCamera* self){
        static std::vector<double> m {16};
        m.resize(16);
        self->getProjectionMatrix(m.data());
        return m;
    });

    c.def("getModelViewMatrix", [](BaseCamera* self){
        static std::vector<double> m {16};
        m.resize(16);
        self->getModelViewMatrix(m.data());
        return m;
    });

    PythonDownCast::registerType<BaseCamera>([](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<BaseCamera*>(object));
    });
}

} /// namespace sofapython3


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
    - paul.scheikl@kit.edu
********************************************************************/

#include <pybind11/pybind11.h>
#include "Binding_SpringForceField.h"
#include "Binding_SpringForceField_doc.h"

#include <SofaDeformable/SpringForceField.h>

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/PythonFactory.h>

using sofa::component::interactionforcefield::LinearSpring;
typedef LinearSpring<SReal> LinearSpringR;

namespace sofapython3 {

namespace py { using namespace pybind11; }

template<class DataType>
void moduleAddSpringForceField(py::module& m) {
    // template the SpringForceField to use it with different types of MechanicalStates such as Vec3 or Rigid3
    using SpringForceField = sofa::component::interactionforcefield::SpringForceField<DataType>;

    // create a python binding for the c++ class SpringForceField from SofaDeformable
    // no init binding, because creation should be done via node.addObject("SpringForceField")
    py::class_<SpringForceField,
            sofa::core::objectmodel::BaseObject,
            py_shared_ptr<SpringForceField>> s (m, "SpringForceField", sofapython3::doc::SofaDeformable::SpringForceFieldClass);

    s.def("clear", &SpringForceField::clear);
    /* s.def("addSpring", &SpringForceField::addSpring) */

    // register the binding in the downcasting subsystem
    PythonFactory::registerType<SpringForceField>([](sofa::core::objectmodel::Base* object){
        return py::cast(dynamic_cast<SpringForceField*>(object));
    });
}
} // namespace sofapython3

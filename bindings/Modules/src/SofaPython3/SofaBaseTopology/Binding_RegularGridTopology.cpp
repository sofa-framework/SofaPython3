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
#include "Binding_RegularGridTopology.h"
#include "Binding_RegularGridTopology_doc.h"

#include <SofaBaseTopology/RegularGridTopology.h>
#include <SofaPython3/PythonFactory.h>

using sofa::component::topology::RegularGridTopology;
using sofa::core::objectmodel::BaseObject;

PYBIND11_DECLARE_HOLDER_TYPE(Base, sofa::core::sptr<Base>, true)

namespace sofapython3 {

namespace py { using namespace pybind11; }

void moduleAddRegularGridTopology(pybind11::module& m) {
    py::class_<RegularGridTopology, sofa::core::objectmodel::BaseObject,sofa::core::sptr<RegularGridTopology>>
    c (m, "RegularGridTopology", doc::SofaBaseTopology::regularGridTopologyClass);

    c.def("getPoint", &RegularGridTopology::getPoint, doc::SofaBaseTopology::getPoint);

    /// register the binding in the downcasting subsystem
    PythonFactory::registerType<RegularGridTopology>([](sofa::core::objectmodel::Base* object){
        return py::cast(dynamic_cast<RegularGridTopology*>(object));
    });
}

} // namespace sofapython3
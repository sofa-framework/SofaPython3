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
#include <pybind11/eval.h>

#include "Binding_Prefab.h"
#include "Binding_Prefab_doc.h"

#include <SofaPython3/DataHelper.h>
#include <SofaPython3/PythonFactory.h>
#include <SofaPython3/PythonEnvironment.h>

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <sofa/core/objectmodel/DataCallback.h>
using sofa::core::objectmodel::DataCallback;

PYBIND11_DECLARE_HOLDER_TYPE(Prefab,
                             sofapython3::py_shared_ptr<Prefab>, true)

#include <sofa/simulation/VisualVisitor.h>
using sofa::simulation::VisualInitVisitor;

#include <sofa/simulation/Simulation.h>
using sofa::simulation::Simulation;

namespace sofapython3
{

class Prefab_Trampoline : public Prefab, public PythonTrampoline
{
public:
    Prefab_Trampoline() = default;

    ~Prefab_Trampoline() override = default;

    std::string getClassName() const override
    {
        return "Prefab"; /// pyobject->ob_type->tp_name;
    }

    void doReInit() override ;
};

void Prefab_Trampoline::doReInit()
{
    if (!m_is_initialized) {
        this->d_componentState.setValue(sofa::core::objectmodel::ComponentState::Loading);
        msg_warning(this) << "Prefab instantiated. Check for required prefab parameters to fully populate";
        return;
    }
    try{
        this->d_componentState.setValue(sofa::core::objectmodel::ComponentState::Valid);
        PYBIND11_OVERLOAD(void, Prefab, doReInit, );
    } catch (std::exception& e)
    {
        this->d_componentState.setValue(sofa::core::objectmodel::ComponentState::Invalid);
        msg_error(this) << e.what();
    }
}

void moduleAddPrefab(py::module &m) {
    py::class_<sofa::core::objectmodel::BasePrefab,
            sofa::simulation::Node,
            sofa::core::objectmodel::BasePrefab::SPtr>(m, "BasePrefab");

    py::class_<Prefab,
            Prefab_Trampoline,
            BasePrefab,
            py_shared_ptr<Prefab>> f(m, "RawPrefab",
                                     py::dynamic_attr(),
                                     py::multiple_inheritance(),
                                     sofapython3::doc::prefab::Prefab);

    f.def(py::init([](py::args& /*args*/, py::kwargs& kwargs){
              auto c = new Prefab_Trampoline();

              for(auto kv : kwargs)
              {

                  std::string key = py::cast<std::string>(kv.first);
                  py::object value = py::reinterpret_borrow<py::object>(kv.second);

                  if( key == "name")
                      c->setName(py::cast<std::string>(kv.second));
                  try {
                      BindingBase::SetAttr(*c, key, value);
                  } catch (py::attribute_error& /*e*/) {
                      /// kwargs are used to set datafields to their initial values,
                      /// but they can also be used as simple python attributes, unrelated to SOFA.
                      /// thus we catch & ignore the py::attribute_error thrown by SetAttr
                  }
              }
              return c;

          }
          ));

    f.def("setSourceTracking", &Prefab::setSourceTracking);
    f.def("addPrefabParameter", &Prefab::addPrefabParameter,
          "name"_a, "help"_a, "type"_a, "default"_a = py::none());
    f.def("init", &Prefab::init);
    f.def("reinit", &Prefab::reinit);
}


}

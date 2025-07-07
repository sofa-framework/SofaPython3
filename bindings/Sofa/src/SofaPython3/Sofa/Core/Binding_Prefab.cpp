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

#include <pybind11/pybind11.h>

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_Prefab.h>
#include <SofaPython3/Sofa/Core/Binding_Prefab_doc.h>

#include <SofaPython3/Prefab.h>
#include <sofa/core/objectmodel/DataCallback.h>
#include <sofa/helper/system/FileMonitor.h>
using sofa::core::objectmodel::DataCallback;

#include <sofa/simulation/VisualVisitor.h>
using sofa::simulation::VisualInitVisitor;

#include <sofa/simulation/Simulation.h>
using sofa::simulation::Simulation;


SOFAPYTHON3_BIND_ATTRIBUTE_ERROR()

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }
/// To bring in the `_a` literal
using namespace pybind11::literals;

namespace sofapython3 {

class Prefab_Trampoline : public Prefab {
public:
    SOFA_CLASS(Prefab_Trampoline, Prefab);

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
            py_shared_ptr<sofa::core::objectmodel::BasePrefab>>(m, "BasePrefab", "Base class for prefabs (for more see Sofa.prefab)");

    py::class_<Prefab,
            Prefab_Trampoline,
            BasePrefab,
            py_shared_ptr<Prefab>> f(m, "RawPrefab",
                                     py::dynamic_attr(),
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

#include "Binding_DataEngine.h"
#include "Binding_DataEngine_doc.h"

#include <SofaPython3/DataHelper.h>
#include <SofaPython3/PythonFactory.h>
#include <pybind11/pybind11.h>

PYBIND11_DECLARE_HOLDER_TYPE(PyDataEngine,
                             sofapython3::py_shared_ptr<PyDataEngine>, true)

namespace sofapython3
{
    using sofa::core::objectmodel::Event;
    using sofa::core::DataEngine;
    using sofa::core::objectmodel::BaseData;
    using sofa::core::objectmodel::BaseObject;
    using sofa::core::objectmodel::DDGNode;

    void PyDataEngine::init() {
        std::cout << "PyDataEngine::init()" << std::endl;
    }

    void PyDataEngine::doUpdate() {
    }

    PyDataEngine::PyDataEngine() {
    }

    PyDataEngine::~PyDataEngine() {
    }

    class DataEngine_Trampoline : public PyDataEngine, public PythonTrampoline
    {
    public:
        DataEngine_Trampoline();

        ~DataEngine_Trampoline() override;

        std::string getClassName() const override
        {
            return pyobject->ob_type->tp_name;
        }
        void init() override ;
        void doUpdate() override ;
    };

    DataEngine_Trampoline::DataEngine_Trampoline() = default;

    DataEngine_Trampoline::~DataEngine_Trampoline() = default;

    void DataEngine_Trampoline::init()
    {
        std::cout << "DataEngine_Trampoline::init()" << std::endl;
        PYBIND11_OVERLOAD(void, PyDataEngine, init, );
    }

    void DataEngine_Trampoline::doUpdate()
    {
        py::object self = py::cast(this);
        if (py::hasattr(self, "update")) {
            py::object fct = self.attr("update");
            try {
                fct();
                return;
            } catch (std::exception& /*e*/) {
                throw py::type_error(this->getName() + ": The DataEngine requires an update method with no parameter and no return type");
            }
        }
        throw py::type_error(this->getName() + " has no update() method.");
    }

    void moduleAddDataEngine(pybind11::module &m)
    {
        py::class_<PyDataEngine,
                DataEngine_Trampoline,
                BaseObject,
                py_shared_ptr<PyDataEngine>> f(m, "DataEngine",
                                               py::dynamic_attr(),
                                               py::multiple_inheritance(), sofapython3::doc::dataengine::DataEngine);

        f.def(py::init([](py::args& /*args*/, py::kwargs& kwargs)
        {
                  auto c = new DataEngine_Trampoline();

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
              }));

        f.def("addInput", &PyDataEngine::addInput, sofapython3::doc::dataengine::addInput);
        f.def("addOutput", &PyDataEngine::addOutput, sofapython3::doc::dataengine::addOutput);
    }

} /// namespace sofapython3

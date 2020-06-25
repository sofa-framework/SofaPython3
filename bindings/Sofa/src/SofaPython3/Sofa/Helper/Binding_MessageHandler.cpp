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

#include "Binding_MessageHandler.h"
#include "Binding_MessageHandler_doc.h"

#include <SofaPython3/DataHelper.h>
#include <SofaPython3/PythonFactory.h>
#include <SofaPython3/PythonEnvironment.h>
using sofapython3::PythonEnvironment;

#include <pybind11/pybind11.h>

#include <sofa/core/objectmodel/Base.h>

PYBIND11_DECLARE_HOLDER_TYPE(PyMessageHandler,
                             sofapython3::py_shared_ptr<PyMessageHandler>, true)

namespace sofapython3
{
    using sofa::core::objectmodel::Event;

    void PyMessageHandler::process(Message& /*m*/) {
    }

    PyMessageHandler::PyMessageHandler() {
    }

    PyMessageHandler::~PyMessageHandler() {
    }

    class MessageHandler_Trampoline : public PyMessageHandler, public PythonTrampoline
    {
    public:
        MessageHandler_Trampoline() = default;

        ~MessageHandler_Trampoline() override = default;
        virtual void process(Message& m) override ;
    };

    void MessageHandler_Trampoline::process(Message& m)
    {
        PythonEnvironment::gil acquire {"MessageHandler"};
        py::object self = py::cast(this);

        if( py::hasattr(self, "process") )
        {
            auto typeStr = [&m]() -> std::string {
                switch (m.type())
                {
                    case Message::Type::Info:
                    return "Info";
                    case Message::Type::Error:
                    return "Error";
                    case Message::Type::Fatal:
                    return "Fatal";
                    case Message::Type::Advice:
                    return "Advice";
                    case Message::Type::Warning:
                    return "Warning";
                    case Message::Type::Deprecated:
                    return "Deprecated";
                    default:
                    return "Other";
                }
            };

            auto component = [&m]() {
                sofa::helper::logging::SofaComponentInfo* nfo = dynamic_cast<sofa::helper::logging::SofaComponentInfo*>(m.componentInfo().get());
                if( nfo != nullptr )
                    return py::cast(nfo->m_component);
                return py::object();
            };

            py::dict msg("type"_a=typeStr,
                         "isEmpty"_a=m.empty(),
                         "sender"_a=m.sender(),
                         "message"_a=m.messageAsString(),
                         "component"_a=component
                    );

            py::object fct = self.attr("process")(msg);
        }
    }


    void moduleAddMessageHandler(py::module &m) {
        py::class_<PyMessageHandler,
                MessageHandler_Trampoline,
                std::unique_ptr<PyMessageHandler>> f(m, "MessageHandler",
                                             py::dynamic_attr(),
                                             py::multiple_inheritance());

        f.def(py::init([]()
        {
            return new MessageHandler_Trampoline();
        }));

        f.def("process", &PyMessageHandler::process);
        f.def("__enter__", [](PyMessageHandler* self){
            PythonEnvironment::gil acquire {"MessageHandler"};
            sofa::helper::logging::MessageDispatcher::addHandler(self);
        });
        f.def("__exit__", [](PyMessageHandler* self){
            PythonEnvironment::gil acquire {"MessageHandler"};
            sofa::helper::logging::MessageDispatcher::rmHandler(self);
        });
    }


}

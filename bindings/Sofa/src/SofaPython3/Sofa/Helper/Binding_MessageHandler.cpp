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

#include <SofaPython3/Sofa/Helper/Binding_MessageHandler.h>
#include <SofaPython3/PythonFactory.h>

#include <SofaPython3/PythonEnvironment.h>

#include <sofa/core/objectmodel/Base.h>

using sofapython3::PythonEnvironment;
using sofa::helper::logging::Message;
using sofa::helper::logging::MessageHandler;
using sofa::core::objectmodel::Base;

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

namespace sofapython3
{
    using sofa::core::objectmodel::Event;

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
            }();

            Base* component = [&m]() -> Base* {
                sofa::helper::logging::SofaComponentInfo* nfo = dynamic_cast<sofa::helper::logging::SofaComponentInfo*>(m.componentInfo().get());
                if( nfo != nullptr )
                    return const_cast<Base*>(nfo->m_component);
                return nullptr;
            }();

            py::dict msg;
            msg["type"] = py::str(typeStr);
            msg["isEmpty"]=py::cast(m.empty());
            msg["sender"]=py::str(m.sender());
            msg["message"]=py::str(m.messageAsString());
            msg["component"]=component ? PythonFactory::toPython(component) : py::none();
            py::object fct = self.attr("process")(msg);
        }
    }


    void moduleAddMessageHandler(py::module &m) {
        py::class_<PyMessageHandler, MessageHandler_Trampoline> f(m, "MessageHandler", py::dynamic_attr());

        f.def(py::init([]()
        {
            return new MessageHandler_Trampoline();
        }));

        f.def("process", &PyMessageHandler::process);
        f.def("__enter__", [](PyMessageHandler* self) -> PyMessageHandler*
        {
            sofa::helper::logging::MessageDispatcher::addHandler(self);
            return self;
        });
        f.def("__exit__", [](PyMessageHandler* self, py::object /*exc_type*/, py::object /*exc_value*/, py::object /*traceback*/)
        {
            sofa::helper::logging::MessageDispatcher::rmHandler(self);
        });
    }


}

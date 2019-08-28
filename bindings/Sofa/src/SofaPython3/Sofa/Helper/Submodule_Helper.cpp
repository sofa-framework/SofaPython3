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


#include <sofa/helper/logging/Messaging.h>
#include <SofaPython3/PythonEnvironment.h>
#include <sofa/core/objectmodel/Base.h>
#include "System/Submodule_System.h"
#include "Submodule_Helper.h"
#include "Binding_Vector.h"

namespace sofapython3
{
using sofa::core::objectmodel::Base;
using sofa::helper::logging::ComponentInfo;
using sofa::helper::logging::SofaComponentInfo;

#define MESSAGE_DISPATCH(MessageType) \
    parse_emitter_message_then(args, [](const ComponentInfo::SPtr& emitter, \
                                  const char* message, \
                                  const sofa::helper::logging::FileInfo::SPtr& fileinfo) \
    { MessageType(emitter) << message << fileinfo; });

static const std::string s_emitter = "PythonScript";

template<class Action>
static void parse_emitter_message_then(py::args args, const Action& action) {
    py::object py_emitter {py::none()};

    const size_t argSize = args.size();
    if( argSize == 1 )
    {
        /// no emitter
        std::string message = py::cast<std::string>(args[0]);
        action(ComponentInfo::SPtr(new ComponentInfo(s_emitter)), message.c_str(),
                                   PythonEnvironment::getPythonCallingPointAsFileInfo());
    } else if( argSize == 2 ) {
        /// SOURCE, "Message"
        py_emitter = args[0];
        std::string message = py::cast<std::string>(args[1]);

        if( py::isinstance<py::str>(py_emitter) )
        {
            action(ComponentInfo::SPtr(new ComponentInfo(py::cast<std::string>(py_emitter))),
                   message.c_str(), PythonEnvironment::getPythonCallingPointAsFileInfo());
        }else if (py::isinstance<Base>(py_emitter))
        {
            action(ComponentInfo::SPtr(new SofaComponentInfo(py::cast<Base*>(py_emitter))),
                   message.c_str(), PythonEnvironment::getPythonCallingPointAsFileInfo());
        }else
        {
            throw py::type_error("The first parameter must be a string or a Sofa.Core.Base object");
        }
    } else if( argSize == 3 ){
        /// "Message", "FILENAME", LINENO
        std::string message = py::cast<std::string>(args[0]);
        std::string filename = py::cast<std::string>(args[1]);
        int lineno = py::cast<int>(args[2]);

        action(ComponentInfo::SPtr(new ComponentInfo(s_emitter)),
               message.c_str(), SOFA_FILE_INFO_COPIED_FROM(filename, lineno));
    } else if (argSize == 4 ){
        /// SOURCE, "Message", "FILENAME", LINENO
        py_emitter = args[0];
        std::string message = py::cast<std::string>(args[1]);
        std::string filename = py::cast<std::string>(args[2]);
        int lineno = py::cast<int>(args[3]);

        if( py::isinstance<std::string>(py_emitter) )
        {
            action(ComponentInfo::SPtr(new ComponentInfo(py::cast<std::string>(py_emitter))),
                   message.c_str(),  SOFA_FILE_INFO_COPIED_FROM(filename, lineno));
        }else if (py::isinstance<Base>(py_emitter))
        {
            action(ComponentInfo::SPtr(new SofaComponentInfo(py::cast<Base*>(py_emitter))),
                   message.c_str(),  SOFA_FILE_INFO_COPIED_FROM(filename, lineno));
        }else
        {
            throw py::type_error("The first parameter must be a string or a Sofa.Core.Base object");
        }
    } else {
        throw py::type_error("Invalid arguments type to function.");
    }
}

/// The first parameter must be named the same as the module file to load.
PYBIND11_MODULE(Helper, helper)
{
    helper.doc() = R"doc(
           Utility functions
           -----------------------

           .. autosummary::
               Sofa.Helper.msg_info
               Sofa.Helper.msg_warning
               Sofa.Helper.msg_error
               Sofa.Helper.msg_deprecated
               Sofa.Helper.msg_fatal

       )doc";

    helper.def("msg_info", [](py::args args) { MESSAGE_DISPATCH(msg_info); },
            R"(Emit an info message from python.
            The 'info' messages are emitted *only* when the object.printLog is
            set to True.

            Examples:
                msg_info("something bad happens")
                msg_info(sofaObject, "something bad happens")
                msg_info(sofaNode, "something bad happens")
                msg_info(emitting_file, emitting_loc, "something bad happens")
                msg_info(sofaObject, "something bad happens", emitting_file, emitting_loc)
                msg_info(sofaNode, "something bad happens", emitting_file, emitting_loc)

            Notes:
                The way the message are printed depends on the application.
                Messages can be routed the console, log files, GUI or ignored.
            )"
            );
    helper.def("msg_warning", [](py::args args) { MESSAGE_DISPATCH(msg_warning); });
    helper.def("msg_error", [](py::args args) { MESSAGE_DISPATCH(msg_error); });
    helper.def("msg_deprecated", [](py::args args) { MESSAGE_DISPATCH(msg_deprecated); });
    helper.def("msg_fatal", [](py::args args) { MESSAGE_DISPATCH(msg_fatal); });

    moduleAddVector(helper);
    moduleAddSystem(helper);
}

} ///namespace sofapython3

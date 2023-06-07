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

#include <sofa/core/init.h>
#include <sofa/helper/init.h>
#include <sofa/helper/logging/Messaging.h>
#include <SofaPython3/PythonEnvironment.h>
#include <sofa/core/objectmodel/Base.h>
#include <SofaPython3/Sofa/Helper/System/Submodule_System.h>
#include <SofaPython3/Sofa/Helper/Binding_MessageHandler.h>
#include <SofaPython3/Sofa/Helper/Binding_Vector.h>

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

namespace sofapython3 {

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

        if( py::isinstance<py::str>(py_emitter) )
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
    // These are needed to force the dynamic loading of module dependencies (found in CMakeLists.txt)
    sofa::core::init();
    sofa::helper::init();

    helper.doc() = R"doc(
            Tooling to print messages or create vectors of different type.

            Sofa provides a fully feature messaging system allowing to emit messages.
            The way the message are printed depends on the application. Messages can be routed the console, log files,
            GUI or ignored.

            There are several kind of messages.

            The 'info' messages are emitted *only* when the object.printLog is set to True.
            The 'warning' messages are emitted when the object want the user to be informed.
            The 'error' messages are emitted when the object cannot perform as expected.
            The 'deprecated' messages are indicating that some feature are now deprecated and thus be fixed as soon as possible.
            In general we provide updates tips with deprecated messages.

            Examples of use
            ---------------

             .. code-block:: python

               msg_error("something bad happens")
               msg_error(sofaObject, "something bad happens")
               msg_warning(sofaNode, "something happens that sofaNode must be award of")
               msg_warning(emitting_file, emitting_loc, "something bad happens at given location")
               msg_info(sofaObject, "something happens", emitting_file, emitting_loc)
               msg_info(sofaNode, "something happens", emitting_file, emitting_loc)

       )doc";

    helper.def("msg_info", [](py::args args) { MESSAGE_DISPATCH(msg_info); },
            R"(Emit an info message from python.)"
            );
    helper.def("msg_warning", [](py::args args) { MESSAGE_DISPATCH(msg_warning); },
    R"(Emit a warning message from python.)");
    helper.def("msg_error", [](py::args args) { MESSAGE_DISPATCH(msg_error); },
    R"(Emit an error message from python.)");
    helper.def("msg_deprecated", [](py::args args) { MESSAGE_DISPATCH(msg_deprecated); },
    R"(Emit a deprecated message from python.)");
    helper.def("msg_fatal", [](py::args args) { MESSAGE_DISPATCH(msg_fatal); },
    R"(Emit a fatal error message from python.)");

    moduleAddMessageHandler(helper);
    moduleAddVector(helper);
    moduleAddSystem(helper);
}

} ///namespace sofapython3

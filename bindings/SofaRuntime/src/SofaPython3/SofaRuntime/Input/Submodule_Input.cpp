#include <sofa/helper/logging/Messaging.h>

#include "Submodule_Input.h"
#include "Submodule_Input_doc.h"


namespace sofapython3
{

py::module addSubmoduleInput(py::module &m)
{
    py::module input = m.def_submodule("Input");

    /// TODO: fill the docstring!
    input.doc() = sofapython3::sofaruntime::input::doc::Input;

    input.def("isKeyPressed", [](int keycode)
    {
        msg_error("SofaPython3::Input") << "Implementation is missing";
        return false;
    }, sofapython3::sofaruntime::input::doc::isKeyPressed);

    input.def("isMouseButtonPressed", [](int mouseButton)
    {
        msg_error("SofaPython3::Input") << "Implementation is missing";
        return false;
    },sofapython3::sofaruntime::input::doc::isMouseButtonPressed);

    input.def("mousePosition", []()
    {
        msg_error("SofaPython3::Input") << "Implementation is missing";
        return std::pair<int, int>{150,150};
    },sofapython3::sofaruntime::input::doc::mousePosition);

    return input;
}

}

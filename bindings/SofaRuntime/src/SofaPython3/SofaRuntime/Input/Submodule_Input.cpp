/*********************************************************************
Copyright 2019, Inria, CNRS, University of Lille

This file is part of runSofa2

runSofa2 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

runSofa2 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with sofaqtquick. If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/
/********************************************************************
 Contributors:
    - damien.marchal@univ-lille.fr
********************************************************************/
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

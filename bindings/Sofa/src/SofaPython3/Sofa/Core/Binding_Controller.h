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

#pragma once
#include "Binding_BaseObject.h"
#include <SofaPython3/DataHelper.h>

#include <sofa/core/behavior/BaseController.h>

namespace sofapython3
{
using sofa::core::behavior::BaseController;

class Controller : public BaseController
{
public:
    SOFA_CLASS(Controller, BaseController);
    void init() override {};
    void reinit() override {};

    Controller() {};
    ~Controller() override {};
};

void moduleAddController(py::module &m);

class Controller_Trampoline : public Controller
{
public:
    Controller_Trampoline() = default;

    ~Controller_Trampoline() override = default;

    void init() override;
    void reinit() override;
    void handleEvent(sofa::core::objectmodel::Event* event) override;

private:
    void callScriptMethod(const py::object& self, sofa::core::objectmodel::Event* event,
        const std::string& methodName);
};

} /// namespace sofapython3


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

#pragma once

#include <pybind11/pybind11.h>
#include <sofa/core/behavior/BaseController.h>
#include <SofaPython3/Sofa/Core/Binding_Component.h>
#include <string>

namespace sofapython3 {

/**
 * Empty controller shell that allows pybind11 to bind init and reinit
 * (BaseController doesn't declare them as virtual).
 */
class Controller : public sofa::core::behavior::BaseController {
public:
    SOFA_CLASS(Controller, sofa::core::behavior::BaseController);
    void init() override {}
    void reinit() override {}
};

class Controller_Trampoline : public Controller, public TrampolineBase
{
public:
    SOFA_CLASS(Controller_Trampoline, Controller);

    Controller_Trampoline();

    void init() override;
    void reinit() override;
    void draw(const sofa::core::visual::VisualParams* params) override;
    void handleEvent(sofa::core::objectmodel::Event* event) override;
    std::string getClassName() const override;
};

void moduleAddController(pybind11::module &m);

} /// namespace sofapython3

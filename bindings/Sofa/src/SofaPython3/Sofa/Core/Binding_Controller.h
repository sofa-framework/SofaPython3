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

namespace sofapython3 {

/**
 * Empty controller shell that allows pybind11 to bind the init and reinit methods (since BaseController doesn't have
 * them)
 */
class Controller : public sofa::core::behavior::BaseController {
public:
    SOFA_CLASS(Controller, sofa::core::behavior::BaseController);
    void init() override {};
    void reinit() override {};
    void bwdInit() override {};
    void storeResetState() override {};
};

class Controller_Trampoline : public Controller
{
public:
    SOFA_CLASS(Controller_Trampoline, Controller);

    void init() override;
    void reinit() override;
    void bwdInit() override;
    void storeResetState() override;
    void handleEvent(sofa::core::objectmodel::Event* event) override;

    std::string getClassName() const override;

private:
    void callScriptMethod(const pybind11::object& self, sofa::core::objectmodel::Event* event,
        const std::string& methodName);
};

void moduleAddController(pybind11::module &m);

} /// namespace sofapython3


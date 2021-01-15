/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2021 INRIA, USTL, UJF, CNRS, MGH                     *
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

#include "Binding_BaseObject_MockComponent.h"
#include <sofa/core/ObjectFactory.h>


Binding_BaseObject_MockComponent::Binding_BaseObject_MockComponent():
d_test(initData(&d_test, std::string(""), "test", "This data should have the value of the last called method in the test."))
{
}
Binding_BaseObject_MockComponent::~Binding_BaseObject_MockComponent()
{

}
void Binding_BaseObject_MockComponent::bwdInit(){
    d_test.setValue("bwdInit");
}
void Binding_BaseObject_MockComponent::cleanup(){
    d_test.setValue("cleanup");
}
void Binding_BaseObject_MockComponent::computeBBox(const sofa::core::ExecParams* /* params */, bool /*onlyVisible*/){
    d_test.setValue("computeBBox");
}

void Binding_BaseObject_MockComponent::storeResetState(){
    d_test.setValue("storeResetState");
}

void Binding_BaseObject_MockComponent::reset(){
    d_test.setValue("reset");
}

void Binding_BaseObject_MockComponent::init(){
    d_test.setValue("init");
}

void Binding_BaseObject_MockComponent::reinit(){
    d_test.setValue("reinit");
}



static int Binding_BaseObject_MockComponentClass = sofa::core::RegisterObject("This component is used to test several functions of Binding_BaseObject")
        .add< Binding_BaseObject_MockComponent >();

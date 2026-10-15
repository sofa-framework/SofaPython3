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

#include "Binding_BaseObject_MockComponent.h"
#include <sofa/core/ObjectFactory.h>


Binding_BaseObject_MockComponent::Binding_BaseObject_MockComponent():
d_test(initData(&d_test, std::string(""), "test", "This data should have the value of the last called method in the test.")),
d_matrixXf(initData(&d_matrixXf, "matrixXf", "Dynamic 2x2 float Eigen matrix.")),
d_matrixXd(initData(&d_matrixXd, "matrixXd", "Dynamic 2x3 double Eigen matrix.")),
d_matrix3d(initData(&d_matrix3d, "matrix3d", "Fixed-size 3x3 double Eigen matrix.")),
d_matrixXi(initData(&d_matrixXi, "matrixXi", "Dynamic 2x2 int Eigen matrix."))
{
    d_matrixXf.setValue(Eigen::MatrixXf::Constant(2, 2, 1.0f));
    d_matrixXd.setValue(Eigen::MatrixXd::Constant(2, 3, 2.0));
    d_matrix3d.setValue(Eigen::Matrix3d::Identity());
    d_matrixXi.setValue(Eigen::MatrixXi::Constant(2, 2, 7));
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

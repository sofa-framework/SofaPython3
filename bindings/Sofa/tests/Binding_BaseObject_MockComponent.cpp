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



int Binding_BaseObject_MockComponentClass = sofa::core::RegisterObject("This component is used to test several functions of Binding_BaseObject")
        .add< Binding_BaseObject_MockComponent >();

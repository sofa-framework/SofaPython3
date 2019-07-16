#pragma once

#include <sofa/core/objectmodel/BaseObject.h>




class Binding_BaseObject_MockComponent : public sofa::core::objectmodel::BaseObject
{
public:
SOFA_CLASS(Binding_BaseObject_MockComponent, sofa::core::objectmodel::BaseObject);
        sofa::core::objectmodel::Data <std::string> d_test;

        Binding_BaseObject_MockComponent ();
        virtual ~Binding_BaseObject_MockComponent () override;
        void bwdInit() override;
        void cleanup() override;
        void computeBBox(const sofa::core::ExecParams* /* params */, bool /*onlyVisible*/=false) override;
        //void setSrc(const std::string &v, const BaseObject *loader, std::vector< std::string > *attributeList=nullptr);
        void storeResetState() override;
        void reset() override;
        void init() override;
        void reinit() override;
};


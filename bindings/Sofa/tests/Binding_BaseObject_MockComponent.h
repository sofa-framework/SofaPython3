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


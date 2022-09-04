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

#pragma once

#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/BaseData.h>

namespace sofapython3
{

/// A placeholder class storing a link to a data or a base
///
/// LinkPath can be used to indicate that a link is needed to be made in place of a string
/// like "@/usr/myobj.position" or "@/usr/myobj"
///
class LinkPath
{
public:
    sofa::core::sptr<sofa::core::objectmodel::Base> targetBase;
    sofa::core::objectmodel::BaseData* targetData;

    LinkPath(sofa::core::sptr<sofa::core::objectmodel::Base>);
    LinkPath(sofa::core::objectmodel::BaseData*);

    bool isPointingToData() const;
};

} /// sofapython3

/******************************************************************************
*                              SofaPython3 plugin                             *
*                  (c) 2019 CNRS, University of Lille, INRIA                  *
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
* Authors: see Authors.txt                                                    *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
/********************************************************************
 Contributors:
    - damien.marchal@univ-lille.fr
    - bruno.josue.marques@inria.fr
    - eve.le-guillou@centrale.centralelille.fr
    - jean-nicolas.brunet@inria.fr
    - thierry.gaugry@inria.fr
********************************************************************/

#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/BaseData.h>
#include <SofaPython3/DataHelper.h>
#include <SofaPython3/PythonFactory.h>

#include "Binding_DataDict_doc.h"


namespace sofapython3 {

class DataDict
{
public:
    sofa::core::sptr<sofa::core::objectmodel::Base> owner;
    DataDict(sofa::core::sptr<sofa::core::objectmodel::Base> b){ owner = b; }
};

class DataDictIterator
{
public:
    sofa::core::sptr<sofa::core::objectmodel::Base> owner;
    size_t     index=0;
    bool       key;
    bool       value;
    DataDictIterator(sofa::core::sptr<sofa::core::objectmodel::Base> owner_, bool withKey, bool withValue)
    {
        owner=owner_;
        index=0;
        key=withKey;
        value=withValue;
    }
};

void moduleAddDataDict(pybind11::module& m);
void moduleAddDataDictIterator(pybind11::module& m);

}  // namespace sofapython3

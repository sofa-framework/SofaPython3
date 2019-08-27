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

#include <sofa/defaulttype/DataTypeInfo.h>
using sofa::defaulttype::AbstractTypeInfo;

#include <sofa/core/objectmodel/BaseData.h>
using sofa::core::objectmodel::BaseData;

#include <sofa/core/objectmodel/BaseObject.h>
using  sofa::core::objectmodel::BaseObject;

#include <sofa/core/objectmodel/BaseNode.h>
using  sofa::core::objectmodel::BaseNode;

#include <SofaPython3/DataHelper.h>
#include <SofaPython3/PythonFactory.h>
#include "../Binding_Base.h"
#include "../Binding_BaseData.h"
#include "Binding_DataString.h"
#include "Binding_DataString_doc.h"

namespace sofapython3
{

py::str DataString::__str__()
{
    std::stringstream s;
    s << "Sofa.Core.DataString<name='" << getName()
                        << "', value='" << getValueString()
                        << "', address='"<< (void*)this <<"'>";
    return s.str();
}

py::str DataString::__repr__()
{
    return py::repr(convertToPython(this));
}

py::bool_ DataString::__eq__(BaseData* other)
{
    if( this == other )
        return true;
    return false;
}

py::size_t DataString::__len__()
{
    return getValueString().size();
}

py::object DataString::__getitem__(const py::size_t index)
{
    auto nfo = getValueTypeInfo();
    return py::str(&nfo->getTextValue(getValueVoidPtr(),0).at(index),1);
}

void moduleAddDataString(py::module& m)
{
    py::class_<DataString, BaseData, raw_ptr<DataString>> s(m, "DataString");

    PythonFactory::registerType<DataString>("DataString", [](BaseData* data) -> py::object {
        return py::cast(reinterpret_cast<DataString*>(data));
    });

    s.def("__eq__", &DataString::__eq__);
    s.def("__len__", &DataString::__len__);
    s.def("__getitem__",&DataString::__getitem__);
    s.def("__repr__",&DataString::__repr__);
    s.def("__str__", &DataString::__str__);
}

}

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
#include "Binding_DataVectorString.h"
#include "Binding_DataVectorString_doc.h"

namespace sofapython3
{

py::str DataVectorString::__str__()
{
    std::stringstream s;
    s << "Sofa.Core.DataVectorString<name='" << getName()
                        << "', value='" << getValueString()
                        << "', address='" << reinterpret_cast<void*>(this) << "'>";
    return s.str();
}

py::str DataVectorString::__repr__()
{
    return py::repr(convertToPython(this));
}

py::size_t DataVectorString::__len__()
{
    return getValueTypeInfo()->size(getValueVoidPtr());
}

py::object DataVectorString::__getitem__(const py::size_t index)
{
    auto nfo = getValueTypeInfo();
    return py::str(nfo->getTextValue(getValueVoidPtr(),index));
}

void moduleAddDataVectorString(py::module& m)
{
    py::class_<DataVectorString, BaseData, std::unique_ptr<DataVectorString, py::nodelete>> s(m, "DataVectorString");

    PythonFactory::registerType("DataVectorString", [](BaseData* data) -> py::object {
        return py::cast(reinterpret_cast<DataVectorString*>(data));
    });

    s.def("__len__", &DataVectorString::__len__);
    s.def("__getitem__",&DataVectorString::__getitem__);
    s.def("__repr__",&DataVectorString::__repr__);
    s.def("__str__", &DataVectorString::__str__);
}

}

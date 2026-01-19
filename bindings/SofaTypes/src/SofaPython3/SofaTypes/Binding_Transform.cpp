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
#include <SofaPython3/SofaTypes/Binding_Transform.h>
#include <sofa/defaulttype/typeinfo/DataTypeInfo.h>
#include <SofaPython3/SofaTypes/Binding_Vec.h>
#include <SofaPython3/SofaTypes/Binding_Quat.h>

namespace pyTransform
{
template<class TReal>
std::string __str__(const sofa::type::Transform<TReal>& self, bool repr)
{
    std::string s;
    if (repr)
    {
        s += "Transform" + sofa::defaulttype::DataTypeInfo<TReal>::name();
    }
    s += "(";
    s += pyVec::__str__(self.getOrigin(), repr);
    s += std::string(", ");
    s += pyQuat::__str__(self.getOrientation(), repr);
    s += ")";
    return s;
}
}

namespace sofapython3::SofaTypes
{

template<class TReal>
void addTransform(py::module& m)
{
    const auto className = "Transform" + sofa::defaulttype::DataTypeInfo<TReal>::name();
    py::class_<sofa::type::Transform<TReal>> p(m, className.c_str());

    p.def("__str__", [](sofa::type::Transform<TReal>& self)
        {
            return pyTransform::__str__(self, false);
        });
    p.def("__repr__", [](sofa::type::Transform<TReal>& self)
        {
            return pyTransform::__str__(self, false);
        });
}

void moduleAddTransform(py::module& m)
{
    addTransform<SReal>(m);
}

}

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
#include <SofaPython3/SofaDeformable/Binding_CorotationalFEMForceField.h>

#include <pybind11/numpy.h>

#include <sofa/component/solidmechanics/fem/elastic/CorotationalFEMForceField.h>
#include <sofa/defaulttype/VecTypes.h>

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/PythonFactory.h>

#include <algorithm>
#include <string>

namespace sofapython3
{
namespace py = pybind11;

template <class DataTypes, class ElementType>
void bindCorotationalFEMForceField(py::module& m, const std::string& elementName)
{
    using ForceField = sofa::component::solidmechanics::fem::elastic::CorotationalFEMForceField<DataTypes, ElementType>;

    const std::string pyclassName = std::string("CorotationalFEMForceField") + DataTypes::Name() + elementName;
    py::class_<ForceField, sofa::core::behavior::ForceField<DataTypes>, py_shared_ptr<ForceField>> f(
        m, pyclassName.c_str(), py::multiple_inheritance());

    f.def("getElementRotations", [](const ForceField& self)
    {
        constexpr py::ssize_t dim = DataTypes::spatial_dimensions;
        const auto& rotations = self.getElementRotations();
        py::array_t<SReal> result({static_cast<py::ssize_t>(rotations.size()), dim, dim});
        SReal* out = result.mutable_data();
        for (const auto& rotation : rotations)
            out = std::copy_n(rotation.ptr(), dim * dim, out);
        return result;
    }, "Rotation of every element, as last computed: shape (nb_elements, spatial_dimension, spatial_dimension).");

    PythonFactory::registerType<ForceField>([](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<ForceField*>(object));
    });
}

void moduleAddCorotationalFEMForceField(py::module& m)
{
    using namespace sofa::defaulttype;
    using namespace sofa::geometry;

    bindCorotationalFEMForceField<Vec2Types, Triangle>(m, "Triangle");
    bindCorotationalFEMForceField<Vec2Types, Quad>(m, "Quad");
    bindCorotationalFEMForceField<Vec3Types, Tetrahedron>(m, "Tetrahedron");
    bindCorotationalFEMForceField<Vec3Types, Hexahedron>(m, "Hexahedron");
}

} // namespace sofapython3

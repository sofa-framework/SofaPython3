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

#include <string>
#include <vector>

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

    f.def("getElementRotations", [pyclassName](py::object pySelf)
    {
        if (!py::isinstance<ForceField>(pySelf))
            throw py::type_error("getElementRotations: argument is not a " + pyclassName);
        const auto& self = pySelf.cast<const ForceField&>();

        constexpr py::ssize_t dim = DataTypes::spatial_dimensions;
        const auto& rotations = self.getElementRotations();
        const py::ssize_t nbElements = static_cast<py::ssize_t>(rotations.size());
        const std::vector<py::ssize_t> shape { nbElements, dim, dim };

        constexpr py::ssize_t bytesToNextValue = sizeof(SReal);
        constexpr py::ssize_t bytesToNextRow = dim * bytesToNextValue;
        constexpr py::ssize_t bytesToNextMatrix = dim * bytesToNextRow;
        const std::vector<py::ssize_t> strides { bytesToNextMatrix, bytesToNextRow, bytesToNextValue };

        const SReal* ptrToStart = rotations.empty() ? nullptr : rotations.front().ptr();

        py::array_t<SReal> rotations_view(shape, strides, ptrToStart, pySelf);
        rotations_view.attr("flags").attr("writeable") = false; // due to getElementRotations constness
        return rotations_view;
    }, "Read-only view on the rotation of every element, as last computed: shape (nb_elements, spatial_dimension, spatial_dimension). "
       "Topological changes invalidate the view.");

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

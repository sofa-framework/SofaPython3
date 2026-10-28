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
#include <SofaPython3/SofaDeformable/Binding_CauchyStress.h>

#include <pybind11/numpy.h>

#include <sofa/component/solidmechanics/fem/elastic/CauchyStressEvaluator.h>
#include <sofa/core/objectmodel/Base.h>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/type/VoigtNotation.h>

#include <string>

// Bindings for CauchyStressEvaluator: the Cauchy stress a force field or a material reports for given
// deformation gradients.

namespace sofapython3
{
namespace py = pybind11;

using RealArray = py::array_t<SReal, py::array::c_style | py::array::forcecast>;

template <class DataTypes>
static py::array_t<SReal> computeCauchyStressFor(sofa::core::objectmodel::Base* component,
                                                const RealArray& deformationGradientsArray)
{
    using Evaluator = sofa::component::solidmechanics::fem::elastic::CauchyStressEvaluator<DataTypes>;
    constexpr py::ssize_t dim = DataTypes::spatial_dimensions;

    auto* evaluator = dynamic_cast<Evaluator*>(component);
    if (!evaluator)
        throw py::type_error("computeCauchyStress: '" + component->getName()
                             + "' is not a CauchyStressEvaluator<" + DataTypes::Name() + ">");

    const auto deformationGradients = deformationGradientsArray.unchecked<4>();   // (nbElements, Q, dim, dim)
    const py::ssize_t nbElements = deformationGradients.shape(0);
    const py::ssize_t Q = deformationGradients.shape(1);

    py::array_t<SReal> stress({nbElements, Q, dim, dim});
    auto stressView = stress.mutable_unchecked<4>();

    typename Evaluator::DeformationGradient F;
    for (py::ssize_t e = 0; e < nbElements; ++e)
    {
        for (py::ssize_t q = 0; q < Q; ++q)
        {
            for (auto i = 0; i < dim; ++i)
                for (auto j = 0; j < dim; ++j)
                    F(i, j) = deformationGradients(e, q, i, j);

            const auto stressVoigt = evaluator->computeStress(F, static_cast<sofa::Size>(e));
            for (sofa::Size v = 0; v < sofa::type::NumberOfIndependentElements<dim>; ++v)
            {
                const auto [i, j] = sofa::type::toTensorIndices<dim>(v);
                stressView(e, q, i, j) = stressVoigt[v];
                stressView(e, q, j, i) = stressVoigt[v];
            }
        }
    }
    return stress;
}

static py::array_t<SReal> computeCauchyStress(sofa::core::objectmodel::Base* component,
                                             RealArray deformationGradients)
{
    using namespace sofa::defaulttype;

    if (!component)
        throw py::value_error("computeCauchyStress: component is None");

    if (deformationGradients.ndim() != 4 || deformationGradients.shape(2) != deformationGradients.shape(3))
        throw py::value_error("computeCauchyStress: deformationGradients must be a 4D array "
                              "(nb_elements, nb_quadrature_points, spatial_dimension, spatial_dimension)");

    switch (deformationGradients.shape(3))
    {
        case 1: return computeCauchyStressFor<Vec1Types>(component, deformationGradients);
        case 2: return computeCauchyStressFor<Vec2Types>(component, deformationGradients);
        case 3: return computeCauchyStressFor<Vec3Types>(component, deformationGradients);
        default:
            throw py::value_error("computeCauchyStress: unsupported dimension "
                                  + std::to_string(deformationGradients.shape(3)));
    }
}

void moduleAddCauchyStress(py::module& m)
{
    m.def("computeCauchyStress", &computeCauchyStress,
          py::arg("component"), py::arg("deformationGradients"),
          "Cauchy stress reported by a CauchyStressEvaluator (a force field or a material) for deformation "
          "gradients of shape (nb_elements, nb_quadrature_points, spatial_dimension, spatial_dimension), "
          "element e being the component's element e: returns the full symmetric stress tensors, same shape.");
}

} // namespace sofapython3

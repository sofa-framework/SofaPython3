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
#include <SofaPython3/SofaFEM/Binding_Quadrature.h>

#include <pybind11/numpy.h>

#include <sofa/config.h>
#include <sofa/fem/FiniteElement[all].h>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/type/Mat.h>

#include <array>
#include <string>

// Bindings for sofa::fem::FiniteElement's reference-element functions.
// Exposes the element kernel (quadrature rule, shape functions, reference gradients) so that an 
// integration loop can be carried out using SofaPython3's API. 

namespace sofapython3
{
namespace py = pybind11;

using RealArray = py::array_t<SReal, py::array::c_style | py::array::forcecast>;

// FiniteElement class dispatch: (element name, ambient dimension)
template <class ElementType, class DataTypes>
struct ElementTag { using FiniteElement = sofa::fem::FiniteElement<ElementType, DataTypes>; };

template <class Function>
static py::tuple withElement(const std::string& element, py::ssize_t dim, Function&& f)
{
    using namespace sofa::defaulttype;
    namespace geo = sofa::geometry;

    if (element == "Edge"        && dim == 1) return f(ElementTag<geo::Edge,        Vec1Types>{});
    if (element == "Edge"        && dim == 2) return f(ElementTag<geo::Edge,        Vec2Types>{});
    if (element == "Edge"        && dim == 3) return f(ElementTag<geo::Edge,        Vec3Types>{});
    if (element == "Triangle"    && dim == 2) return f(ElementTag<geo::Triangle,    Vec2Types>{});
    if (element == "Triangle"    && dim == 3) return f(ElementTag<geo::Triangle,    Vec3Types>{});
    if (element == "Quad"        && dim == 2) return f(ElementTag<geo::Quad,        Vec2Types>{});
    if (element == "Quad"        && dim == 3) return f(ElementTag<geo::Quad,        Vec3Types>{});
    if (element == "Tetrahedron" && dim == 3) return f(ElementTag<geo::Tetrahedron, Vec3Types>{});
    if (element == "Hexahedron"  && dim == 3) return f(ElementTag<geo::Hexahedron,  Vec3Types>{});

    throw py::value_error("SofaFEM: unsupported element '" + element + "' for dimension " + std::to_string(dim));
}

// Reference-space quadrature points' data for one FiniteElement specialization:
//  - weights
//  - shape functions' values N_a
//  - shape functions' gradients dN_a/dxi
template <class FE>
static py::tuple quadratureDataFor(sofa::Size degree)
{
    constexpr py::ssize_t nbNodes = FE::NumberOfNodesInElement;
    constexpr py::ssize_t topoDim = FE::TopologicalDimension;

    const auto rule = FE::quadratureRule(degree);
    const py::ssize_t Q = static_cast<py::ssize_t>(rule.size());

    using Real = typename FE::Real;

    py::array_t<Real> weights({Q});
    py::array_t<Real> shapeFunctions({Q, nbNodes});
    py::array_t<Real> shapeFunctionGrads({Q, nbNodes, topoDim});
    auto weightsView = weights.template mutable_unchecked<1>();
    auto functionsView = shapeFunctions.template mutable_unchecked<2>();
    auto gradientsView = shapeFunctionGrads.template mutable_unchecked<3>();

    py::ssize_t q = 0;
    for (const auto& [referencePoint, weight] : rule)
    {
        weightsView(q) = weight;

        const auto N = FE::shapeFunctions(referencePoint);
        for (auto a = 0; a < nbNodes; ++a)
            functionsView(q, a) = N[a];

        const auto gradient = FE::gradientShapeFunctions(referencePoint);   // Mat<nbNodes, topoDim>
        for (auto a = 0; a < nbNodes; ++a)
            for (auto j = 0; j < topoDim; ++j)
                gradientsView(q, a, j) = gradient[a][j];

        ++q;
    }
    return py::make_tuple(weights, shapeFunctions, shapeFunctionGrads);
}

static py::tuple quadratureData(const std::string& element, py::ssize_t dim, sofa::Size degree)
{
    return withElement(element, dim, [&](auto tag)
    {
        return quadratureDataFor<typename decltype(tag)::FiniteElement>(degree);
    });
}

// Physical-space shape function gradients dN_a/dx and the integration measure per quadrature point
//  - physical space gradients dN_a/dxi
//  - measures
template <class FE>
static py::tuple elementMappingFor(const RealArray& nodeCoordinatesArray, const RealArray& referenceGradientsArray)
{
    using Real = typename FE::Real;
    using Coord = typename FE::Coord;
    using Helper = typename FE::Helper;
    constexpr py::ssize_t nbNodes = FE::NumberOfNodesInElement;
    constexpr py::ssize_t spatialDim = FE::spatial_dimensions;
    constexpr py::ssize_t topoDim = FE::TopologicalDimension;

    const auto nodeCoordinates = nodeCoordinatesArray.unchecked<2>();     // (nbNodes, spatialDim)
    const auto referenceGrads = referenceGradientsArray.unchecked<3>();   // (Q, nbNodes, topoDim)
    const py::ssize_t Q = referenceGrads.shape(0);

    std::array<Coord, FE::NumberOfNodesInElement> elementNodes;
    for (auto a = 0; a < nbNodes; ++a)
        for (auto d = 0; d < spatialDim; ++d)
            elementNodes[a][d] = nodeCoordinates(a, d);

    py::array_t<Real> physicalGradients({Q, nbNodes, spatialDim});
    py::array_t<Real> measures({Q});
    auto gradientsView = physicalGradients.template mutable_unchecked<3>();
    auto measuresView = measures.template mutable_unchecked<1>();

    for (py::ssize_t q = 0; q < Q; ++q)
    {
        sofa::type::Mat<FE::NumberOfNodesInElement, FE::TopologicalDimension, Real> referenceGradient;
        for (auto a = 0; a < nbNodes; ++a)
            for (auto j = 0; j < topoDim; ++j)
                referenceGradient[a][j] = referenceGrads(q, a, j);

        const auto jacobian = Helper::jacobianFromReferenceToPhysical(elementNodes, referenceGradient);
        measuresView(q) = sofa::type::absGeneralizedDeterminant(jacobian);   // |det J|, or sqrt(det(J^T J)) if embedded
        const auto inverseJacobian = sofa::type::inverse(jacobian);          // inverse, or left pseudo-inverse if embedded

        for (auto a = 0; a < nbNodes; ++a)
        {
            const auto physicalGradient = inverseJacobian.transposed() * referenceGradient[a];   // dN_a/dx
            for (auto d = 0; d < spatialDim; ++d)
                gradientsView(q, a, d) = physicalGradient[d];
        }
    }
    return py::make_tuple(physicalGradients, measures);
}

static py::tuple elementMapping(const std::string& element, RealArray nodeCoordinates, RealArray referenceGradients)
{
    if (nodeCoordinates.ndim() != 2)
        throw py::value_error("element_mapping: node_coordinates must be a 2D array (nodes_per_element, spatial_dimension)");

    return withElement(element, nodeCoordinates.shape(1), [&](auto tag)
    {
        return elementMappingFor<typename decltype(tag)::FiniteElement>(nodeCoordinates, referenceGradients);
    });
}

void moduleAddQuadrature(py::module& m)
{
    m.def("quadrature_data", &quadratureData,
          py::arg("element"), py::arg("dim"), py::arg("degree"),
          "Reference-space quadrature data for the element at the given degree: "
          "returns (quadrature weights, shape functions, shape function gradients).");

    m.def("element_mapping", &elementMapping,
          py::arg("element"), py::arg("node_coordinates"), py::arg("reference_gradients"),
          "Reference->physical mapping for one element (reuses SOFA's jacobianFromReferenceToPhysical, "
          "inverse and absGeneralizedDeterminant): returns (physical shape-function gradients dN_a/dx, "
          "integration measures) per quadrature point; handles square and embedded (rectangular Jacobian) elements.");
}

} // namespace sofapython3

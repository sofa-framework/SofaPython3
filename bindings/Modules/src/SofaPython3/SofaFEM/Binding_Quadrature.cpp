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

#include <string>

// Bindings for sofa::fem::FiniteElement's reference-element functions.
// Exposes the element kernel (quadrature rule, shape functions, reference gradients) so that an 
// integration loop can be carried out using SofaPython3's API. 

namespace sofapython3
{
namespace py = pybind11;

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

// Dispatch from element-type name to FiniteElement. The DataTypes is in practice a placeholder.
static py::tuple quadratureData(const std::string& element, sofa::Size degree)
{
    using sofa::fem::FiniteElement;
    using sofa::defaulttype::Vec3Types;
    namespace geo = sofa::geometry;

    if (element == "Edge")        return quadratureDataFor<FiniteElement<geo::Edge,        Vec3Types>>(degree);
    if (element == "Triangle")    return quadratureDataFor<FiniteElement<geo::Triangle,    Vec3Types>>(degree);
    if (element == "Quad")        return quadratureDataFor<FiniteElement<geo::Quad,        Vec3Types>>(degree);
    if (element == "Tetrahedron") return quadratureDataFor<FiniteElement<geo::Tetrahedron, Vec3Types>>(degree);
    if (element == "Hexahedron")  return quadratureDataFor<FiniteElement<geo::Hexahedron,  Vec3Types>>(degree);

    throw py::value_error("SofaFEM: unsupported element type '" + element + "'");
}

void moduleAddQuadrature(py::module& m)
{
    m.def("quadrature_data", &quadratureData,
          py::arg("element"), py::arg("degree"),
          "Reference-space quadrature data for the element at the given degree: "
          "returns (quadrature weights, shape functions, shape function gradients).");
}

} // namespace sofapython3

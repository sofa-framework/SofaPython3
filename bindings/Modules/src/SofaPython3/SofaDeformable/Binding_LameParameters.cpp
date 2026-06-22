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

#include <pybind11/pybind11.h>

#include <SofaPython3/SofaDeformable/Binding_LameParameters.h>
#include <sofa/component/solidmechanics/fem/elastic/impl/LameParameters.h>

namespace sofapython3
{

void moduleAddLameParameters(pybind11::module &m)
{
    using namespace sofa::component::solidmechanics::fem::elastic;

    m.def(
        "toLameParameters2D",
        [](SReal youngModulus, SReal poissonRatio) {
            LameLambda<SReal> lambda{0};
            LameMu<SReal> mu{0};
            toLameParameters<2>(YoungModulus<SReal>(youngModulus),
                                PoissonRatio<SReal>(poissonRatio), lambda, mu);
            return std::make_pair(mu.get(), lambda.get());
        },
        pybind11::arg("youngModulus"), pybind11::arg("poissonRatio"),
        "Converts Young's modulus and Poisson's ratio to Lamé parameters (mu, "
        "lambda) for 2D.");

    m.def(
        "toLameParameters3D",
        [](SReal youngModulus, SReal poissonRatio) {
            LameLambda<SReal> lambda{0};
            LameMu<SReal> mu{0};
            toLameParameters<3>(YoungModulus<SReal>(youngModulus),
                                PoissonRatio<SReal>(poissonRatio), lambda, mu);
            return std::make_pair(mu.get(), lambda.get());
        },
        pybind11::arg("youngModulus"), pybind11::arg("poissonRatio"),
        "Converts Young's modulus and Poisson's ratio to Lamé parameters (mu, "
        "lambda) for 3D.");
}

} // namespace sofapython3

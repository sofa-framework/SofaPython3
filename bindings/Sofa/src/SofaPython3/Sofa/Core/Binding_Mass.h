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

#pragma once

#include <sofa/core/behavior/BaseMass.h>
#include <sofa/core/behavior/Mass.h>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/core/MechanicalParams.h>

namespace sofapython3 {

template<class TDOFType>
class Mass_Trampoline : public sofa::core::behavior::Mass<TDOFType> {
public:
    SOFA_CLASS(Mass_Trampoline, SOFA_TEMPLATE(sofa::core::behavior::Mass, TDOFType));
    using sofa::core::behavior::Mass<TDOFType>::mstate;
    using sofa::core::behavior::Mass<TDOFType>::getContext;
    using typename sofa::core::behavior::Mass<TDOFType>::DataTypes;
    using typename sofa::core::behavior::Mass<TDOFType>::Coord;
    using typename sofa::core::behavior::Mass<TDOFType>::DataVecDeriv;
    using typename sofa::core::behavior::Mass<TDOFType>::DataVecCoord;

    Mass_Trampoline();
    ~Mass_Trampoline() override;

    std::string getClassName() const override;
    void init() override;
};

void moduleAddMass(pybind11::module &m);

} /// namespace sofapython3

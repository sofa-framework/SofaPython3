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

#include <sofa/core/BaseMapping.h>
#include <sofa/core/Mapping.h>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/core/MechanicalParams.h>
#include <pybind11/pybind11.h>

namespace sofapython3 {
using sofa::core::ConstraintParams;
using sofa::core::MechanicalParams;

template<class In, class Out>
class Mapping_Trampoline : public sofa::core::Mapping<In, Out> {
public:
    SOFA_CLASS(SOFA_TEMPLATE2(Mapping_Trampoline, In, Out) ,
               SOFA_TEMPLATE2(sofa::core::Mapping, In, Out));
    using sofa::core::Mapping<In,Out>::getContext;
    using typename sofa::core::Mapping<In,Out>::OutDataVecCoord;
    using typename sofa::core::Mapping<In,Out>::OutDataVecDeriv;
    using typename sofa::core::Mapping<In,Out>::InDataVecCoord;
    using typename sofa::core::Mapping<In,Out>::InDataVecDeriv;
    using typename sofa::core::Mapping<In,Out>::OutDataMatrixDeriv;
    using typename sofa::core::Mapping<In,Out>::InDataMatrixDeriv;

    Mapping_Trampoline();
    ~Mapping_Trampoline() override;

    void init() override;
    std::string getClassName() const override;

    void apply( const MechanicalParams* mparams, OutDataVecCoord& out, const InDataVecCoord& in) override;
    void applyJ( const MechanicalParams* mparams, OutDataVecDeriv& out, const InDataVecDeriv& in) override;
    void applyJT( const MechanicalParams* mparams, InDataVecDeriv& out, const OutDataVecDeriv& in) override;
    void applyJT( const ConstraintParams* mparams, InDataMatrixDeriv& out, const OutDataMatrixDeriv& in) override;

};

void moduleAddMapping(pybind11::module &m);

} /// namespace sofapython3

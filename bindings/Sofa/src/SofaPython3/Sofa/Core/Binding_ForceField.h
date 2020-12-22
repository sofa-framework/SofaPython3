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

#pragma once

#include <sofa/core/behavior/BaseForceField.h>
#include <sofa/core/behavior/ForceField.h>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/core/MechanicalParams.h>

namespace sofapython3 {

template<class TDOFType>
class ForceField_Trampoline  : public sofa::core::behavior::ForceField<TDOFType> {
public:
    SOFA_CLASS(ForceField_Trampoline, SOFA_TEMPLATE(sofa::core::behavior::ForceField, TDOFType));
    using sofa::core::behavior::ForceField<TDOFType>::mstate;
    using sofa::core::behavior::ForceField<TDOFType>::getContext;
    using typename sofa::core::behavior::ForceField<TDOFType>::DataTypes;
    using typename sofa::core::behavior::ForceField<TDOFType>::Coord;
    using typename sofa::core::behavior::ForceField<TDOFType>::DataVecDeriv;
    using typename sofa::core::behavior::ForceField<TDOFType>::DataVecCoord;

    ForceField_Trampoline();
    ~ForceField_Trampoline() override;

    void init() override;

    void addForce(const sofa::core::MechanicalParams* mparams, DataVecDeriv& f, const DataVecCoord& x, const DataVecDeriv& v) override;
    void addDForce(const sofa::core::MechanicalParams* mparams, DataVecDeriv& df, const DataVecDeriv& dx ) override;

    pybind11::object _addKToMatrix(const sofa::core::MechanicalParams* mparams, int nNodes, int nDofs);
    void addKToMatrix(const sofa::core::MechanicalParams* mparams, const sofa::core::behavior::MultiMatrixAccessor* dfId) override;

    SReal getPotentialEnergy(const sofa::core::MechanicalParams* /*mparams*/, const DataVecCoord& /*x*/) const override { return 0.0; }

};

void moduleAddForceField(pybind11::module &m);

} /// namespace sofapython3

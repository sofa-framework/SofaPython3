#pragma once

#include <sofa/core/behavior/BaseForceField.h>
#include <sofa/core/behavior/ForceField.h>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/core/MechanicalParams.h>

#include "Binding_BaseObject.h"
#include <SofaPython3/DataHelper.h>

namespace sofapython3
{
class SOFAPYTHON3_API ForceField_Trampoline  : public sofa::core::behavior::ForceField<sofa::defaulttype::Vec3dTypes>, public PythonTrampoline
{
public:
    ForceField_Trampoline();
    ~ForceField_Trampoline() override;

    void init() override;

    void addForce(const sofa::core::MechanicalParams* mparams,  DataVecDeriv& f, const DataVecCoord& x, const DataVecDeriv& v) override;

    void addDForce(const sofa::core::MechanicalParams* mparams, DataVecDeriv& df, const DataVecDeriv& dx ) override;

    py::object _addKToMatrix(const sofa::core::MechanicalParams* mparams, int nNodes, int nDofs);
    void addKToMatrix(const sofa::core::MechanicalParams* mparams, const sofa::core::behavior::MultiMatrixAccessor* dfId) override;

    SReal getPotentialEnergy(const sofa::core::MechanicalParams* /*mparams*/, const DataVecCoord& /*x*/) const override { return 0.0; }

    std::string getClassName() const override;
};

void moduleAddForceField(py::module &m);

} /// namespace sofapython3

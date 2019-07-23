#pragma once

#include "Binding_BaseObject.h"

#include <sofa/core/behavior/BaseForceField.h>

namespace sofapython3
{
using sofa::core::behavior::BaseForceField;
using sofa::core::behavior::MultiMatrixAccessor;
using sofa::core::MechanicalParams;
using sofa::core::MultiVecDerivId;

void moduleAddForceField(py::module &m);

} /// namespace sofapython3

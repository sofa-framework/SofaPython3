#pragma once

#include "Binding_BaseObject.h"

#include <sofa/core/behavior/BaseForceField.h>


//template class pybind11::class_<
//                          sofa::core::behavior::BaseForceField,
//                          sofa::core::objectmodel::BaseObject,
//                          sofa::core::sptr<sofa::core::behavior::BaseForceField>>;

namespace sofapython3
{
using sofa::core::behavior::BaseForceField;
using sofa::core::behavior::MultiMatrixAccessor;
using sofa::core::MechanicalParams;
using sofa::core::MultiVecDerivId;

void moduleAddForceField(py::module &m);

} /// namespace sofapython3

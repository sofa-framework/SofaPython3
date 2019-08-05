#pragma once

#include "Binding_BaseObject.h"

#include <sofa/core/DataEngine.h>
#include <pybind11/pybind11.h>

template class pybind11::class_<sofa::core::DataEngine,
                                sofa::core::objectmodel::BaseObject,
                                sofa::core::sptr<sofa::core::DataEngine>>;

namespace sofapython3
{
using sofa::core::DataEngine;
using sofa::core::objectmodel::BaseData;
using sofa::core::objectmodel::BaseObject;
using sofa::core::objectmodel::DDGNode;

class PyDataEngine : public DataEngine
{
public:
    SOFA_CLASS(PyDataEngine, DataEngine);
    void init() override;
    void doUpdate() override;

    PyDataEngine();
    ~PyDataEngine() override;

};

void moduleAddDataEngine(pybind11::module &m);

} /// namespace sofapython3

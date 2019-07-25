#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/BaseData.h>
#include <SofaPython3/DataHelper.h>
#include <SofaPython3/PythonDownCast.h>

#include "Binding_DataDict_doc.h"


namespace sofapython3
{
/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

using namespace pybind11::literals;
using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::BaseData;
using sofa::core::sptr;
class DataDict
{
public:
    sptr<Base> owner;
    DataDict(sptr<Base> b){ owner = b; }
};

class DataDictIterator
{
public:
    Base::SPtr owner;
    size_t     index=0;
    bool       key;
    bool       value;
    DataDictIterator(Base::SPtr owner_, bool withKey, bool withValue)
    {
        owner=owner_;
        index=0;
        key=withKey;
        value=withValue;
    }
};

void moduleAddDataDict(py::module& m);
void moduleAddDataDictIterator(py::module& m);

}  // namespace sofapython3

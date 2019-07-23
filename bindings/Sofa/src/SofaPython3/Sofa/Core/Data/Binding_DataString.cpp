#include <sofa/defaulttype/DataTypeInfo.h>
using sofa::defaulttype::AbstractTypeInfo;

#include <sofa/core/objectmodel/BaseData.h>
using sofa::core::objectmodel::BaseData;

#include <sofa/core/objectmodel/BaseObject.h>
using  sofa::core::objectmodel::BaseObject;

#include <sofa/core/objectmodel/BaseNode.h>
using  sofa::core::objectmodel::BaseNode;

#include <SofaPython3/DataHelper.h>
#include "../Binding_Base.h"
#include "../Binding_BaseData.h"
#include "Binding_DataString.h"
#include "Binding_DataString_doc.h"

namespace sofapython3
{

void moduleAddDataString(py::module& m)
{
    py::class_<DataString, BaseData, raw_ptr<DataString>> s(m, "DataString");

    getBindingDataFactoryInstance()->registerCreator(
                "DataString", new TypeCreator<DataString*>());

    s.def("__getitem__", [](BaseData& self, py::size_t index) -> py::object
    {
        auto nfo = self.getValueTypeInfo();
        return py::str(&nfo->getTextValue(self.getValueVoidPtr(),0).at(index),1);
    });
}

}

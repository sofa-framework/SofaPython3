#include "DataSymlink.h"

namespace sofa::core::objectmodel
{


DataSymlink::DataSymlink(BaseData *parent, BaseData* link)
    : BaseData(parent->getName(), false, false),
      m_parent(parent),
      m_link(link)
{
    addInput(m_link);
    setOwnerClass("DataLink");
    setName(parent->getName());
}

bool DataSymlink::read(const std::string &value)
{
    SOFA_UNUSED(value);
    return false;
}

void DataSymlink::printValue(std::ostream &os) const
{
    return m_parent->printValue(os);
}

std::string DataSymlink::getValueString() const
{
    return m_parent->getValueString();
}

std::string DataSymlink::getValueTypeString() const
{
    return "DataSymlink";
}

const sofa::defaulttype::AbstractTypeInfo *DataSymlink::getValueTypeInfo() const
{
    return m_parent->getValueTypeInfo();
}

const void *DataSymlink::getValueVoidPtr() const
{
    return m_parent->getValueVoidPtr();
}

void *DataSymlink::beginEditVoidPtr()
{
    return m_parent->beginEditVoidPtr();
}

void DataSymlink::endEditVoidPtr()
{
    return m_parent->endEditVoidPtr();
}

bool DataSymlink::isCounterValid() const
{
    return true;
}

std::string DataSymlink::getLinkPath() const
{
    return m_link->getOwner()->getPathName() + "." + m_link->getName() + "." + getName();
}


} // namespace sofa::core::objectmodel

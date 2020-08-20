#include "DataAlias.h"

namespace sofa::core::objectmodel
{


DataAlias::DataAlias(BaseData *parent, BaseData* link)
    : BaseData(parent->getName(), false, false),
      m_parent(parent),
      m_link(link)
{
    addInput(m_link);
    setOwnerClass("DataLink");
    setName(parent->getName());
}

bool DataAlias::read(const std::string &value)
{
    SOFA_UNUSED(value);
    return false;
}

void DataAlias::printValue(std::ostream &os) const
{
    return m_parent->printValue(os);
}

std::string DataAlias::getValueString() const
{
    return m_parent->getValueString();
}

std::string DataAlias::getValueTypeString() const
{
    return "DataAlias";
}

const sofa::defaulttype::AbstractTypeInfo *DataAlias::getValueTypeInfo() const
{
    return m_parent->getValueTypeInfo();
}

const void *DataAlias::getValueVoidPtr() const
{
    return m_parent->getValueVoidPtr();
}

void *DataAlias::beginEditVoidPtr()
{
    return m_parent->beginEditVoidPtr();
}

void DataAlias::endEditVoidPtr()
{
    return m_parent->endEditVoidPtr();
}

bool DataAlias::isCounterValid() const
{
    return true;
}

std::string DataAlias::getLinkPath() const
{
    return m_link->getOwner()->getPathName() + "." + m_link->getName() + "." + getName();
}


} // namespace sofa::core::objectmodel

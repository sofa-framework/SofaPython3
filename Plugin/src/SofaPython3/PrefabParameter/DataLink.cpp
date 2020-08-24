#include "DataLink.h"
#include "DataSymlink.h"

namespace sofa {
namespace core {
namespace objectmodel {

DataLink::DataLink(const std::string &helpMsg, bool isDisplayed, bool isReadOnly)
    : Inherit(helpMsg, isDisplayed, isReadOnly)
{
}

DataLink::DataLink(const std::string &value, const std::string &helpMsg, bool isDisplayed, bool isReadOnly)
    : Inherit(value, helpMsg, isDisplayed, isReadOnly)
{
}

DataLink::DataLink(const BaseData::BaseInitData &init)
    : Inherit(init)
{
}

DataLink::~DataLink()
{
    for (auto [k,v] : m_dataSymlinks)
        delete v;
    m_dataSymlinks.clear();
}

const PrefabLink& DataLink::getValue() const
{
    updateIfDirty();
    if (m_value.getValue().getTargetBase()) return m_value.getValue();

    auto self = const_cast<DataLink*>(this);

    Base* dst = nullptr;
    this->getOwner()->findLinkDest(dst, self->m_value.getValue().getTargetPath(), nullptr);
    if (dst) {
        auto edit = self->m_value.beginEdit();
        edit->setTargetBase(dst);
        edit->setTargetPath("");
        self->m_value.endEdit();
    }
    return m_value.getValue();
}

std::string DataLink::getValueString() const
{
    const auto& ptr = getValue();
    if (ptr.getTargetBase())
    {
        auto bn = ptr.getTargetBase()->toBaseNode();
        auto bo = ptr.getTargetBase()->toBaseObject();
        return "@" + (bn ? bn->getPathName() : bo->getPathName());
    }
    return ptr.getTargetPath();
}

bool DataLink::read(const std::string &value)
{
    Base* dst;
    auto data = beginEdit();
    for (auto [k, v] : m_dataSymlinks)
        delete v;
    m_dataSymlinks.clear();
    if (this->getOwner()->findLinkDest(dst, value, nullptr) && dst != nullptr)
    {
        data->setTargetBase(dst);
    }
    else
    {
        data->setTargetBase(nullptr);
        data->setTargetPath(value);
    }
    endEdit();

    return true;
}

BaseData* DataLink::createDataSymlink(const std::string &dataName)
{
    auto base = getValue().getTargetBase().get();
    BaseData* data;
    if (!base)
    {
        this->read(this->getValue().getTargetPath());
    }
    if (base && (data = base->findData(dataName)) != nullptr)
    {
        DataSymlink* alias = new DataSymlink(data, this);
        m_dataSymlinks[dataName] = alias;
        return alias;
    }
    return nullptr;
}


BaseData* DataLink::findDataSymlink(const std::string &dataName)
{
    auto it = m_dataSymlinks.find(dataName);
    if (it != m_dataSymlinks.end())
    {
        return it->second;
    }
    else
    {
       return createDataSymlink(dataName);
    }
}




} // namespace objectmodel
} // namespace core
} // namespace sofa

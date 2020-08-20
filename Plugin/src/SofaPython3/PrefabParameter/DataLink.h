#pragma once

#include "../DataHelper.h"

namespace sofa::core::objectmodel
{

class DataAlias;
class SOFAPYTHON3_API DataLink : public Data<PrefabLink>
{
public:
    typedef Data<PrefabLink> Inherit;

    DataLink( const std::string& helpMsg="", bool isDisplayed=true, bool isReadOnly=false );

    DataLink( const std::string& value, const std::string& helpMsg="", bool isDisplayed=true, bool isReadOnly=false );

    explicit DataLink(const BaseData::BaseInitData& init);

    ~DataLink();

    const PrefabLink& getValue() const;

    std::string getValueString() const;

    bool read(const std::string& value);

    BaseData* createDataAlias(const std::string& dataName);
    BaseData* findDataAlias(const std::string& dataName);

private:
    sofa::core::DataTrackerCallback m_callback;

    std::map<std::string, DataAlias*> m_dataAliases;

};

} // namespace sofa::core::objectmodel


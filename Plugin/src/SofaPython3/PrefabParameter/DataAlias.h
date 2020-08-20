#pragma once

#include "../DataHelper.h"

namespace sofa::core::objectmodel {


// "Fake" data, used in DataLink to provide an interface to the target's datafields
class SOFAPYTHON3_API DataAlias : public BaseData
{
public:
    DataAlias(BaseData* parent, BaseData* link);

    bool read(const std::string& value) override;

    void printValue(std::ostream& os) const override;

    std::string getValueString() const override;

    std::string getValueTypeString() const override;

    const sofa::defaulttype::AbstractTypeInfo* getValueTypeInfo() const override;

    const void* getValueVoidPtr() const override;

    void* beginEditVoidPtr() override;

    void endEditVoidPtr() override;

    bool isCounterValid() const override;

    std::string getLinkPath() const override;

    BaseData* m_parent;
private:
    BaseData* m_link;
};

} // namespace sofa::core::objectmodel

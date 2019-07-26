#pragma once

#include <sofa/helper/Factory.h>
#include <sofa/helper/Factory.inl>
#include <sofa/core/objectmodel/BaseData.h>
using sofa::core::objectmodel::BaseData;

namespace sofapython3
{

typedef sofa::helper::Factory< std::string, BaseData> DataFactory;

DataFactory* getFactoryInstance();

}  // namespace sofapython3

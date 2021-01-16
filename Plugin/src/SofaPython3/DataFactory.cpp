/******************************************************************************
*                              SofaPython3 plugin                             *
*                  (c) 2021 CNRS, University of Lille, INRIA                  *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#include "DataFactory.h"

#include <sofa/core/topology/Topology.h>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/core/objectmodel/BaseData.h>
using sofa::core::objectmodel::BaseData;

namespace sofapython3
{

template<typename DataType>
class DataCreator : public sofa::helper::BaseCreator<sofa::core::objectmodel::BaseData>
{
public:
    virtual sofa::core::objectmodel::BaseData* createInstance(sofa::helper::NoArgument) override { return new sofa::core::objectmodel::Data<DataType>(); }
    virtual const std::type_info& type() override { return typeid(sofa::core::objectmodel::BaseData);}
};

typedef sofa::helper::Factory< std::string, BaseData> DataFactory;

DataFactory* getFactoryInstance()
{
    static DataFactory* s_localfactory = nullptr ;
    if (s_localfactory == nullptr)
    {
        // helper vector style containers
        std::string containers[] = {"vector"};

        s_localfactory = new DataFactory();
        // Scalars
        s_localfactory->registerCreator("string", new DataCreator<std::string>());
        s_localfactory->registerCreator("float", new DataCreator<float>());
        s_localfactory->registerCreator("double", new DataCreator<double>());
        s_localfactory->registerCreator("bool", new DataCreator<bool>());
        s_localfactory->registerCreator("int", new DataCreator<int>());

        // vectors
        s_localfactory->registerCreator(
                    "Vec2d", new DataCreator<sofa::defaulttype::Vec2d>());
        s_localfactory->registerCreator(
                    "Vec3d", new DataCreator<sofa::defaulttype::Vec3d>());
        s_localfactory->registerCreator(
                    "Vec4d", new DataCreator<sofa::defaulttype::Vec4d>());
        s_localfactory->registerCreator(
                    "Vec6d", new DataCreator<sofa::defaulttype::Vec6d>());
        s_localfactory->registerCreator(
                    "Vec2f", new DataCreator<sofa::defaulttype::Vec2f>());
        s_localfactory->registerCreator(
                    "Vec3f", new DataCreator<sofa::defaulttype::Vec3f>());
        s_localfactory->registerCreator(
                    "Vec4f", new DataCreator<sofa::defaulttype::Vec4f>());
        s_localfactory->registerCreator(
                    "Vec6f", new DataCreator<sofa::defaulttype::Vec6f>());

        // Matrices
        s_localfactory->registerCreator(
                    "Mat2x2d", new DataCreator<sofa::defaulttype::Mat2x2d>());
        s_localfactory->registerCreator(
                    "Mat3x3d", new DataCreator<sofa::defaulttype::Mat3x3d>());
        s_localfactory->registerCreator(
                    "Mat4x4d", new DataCreator<sofa::defaulttype::Mat4x4d>());
        s_localfactory->registerCreator(
                    "Mat2x2f", new DataCreator<sofa::defaulttype::Mat2x2f>());
        s_localfactory->registerCreator(
                    "Mat3x3f", new DataCreator<sofa::defaulttype::Mat3x3f>());
        s_localfactory->registerCreator(
                    "Mat4x4f", new DataCreator<sofa::defaulttype::Mat4x4f>());

        // Topology
        s_localfactory->registerCreator("Edge", new DataCreator<sofa::core::topology::Topology::Edge>());
        s_localfactory->registerCreator("Triangle", new DataCreator<sofa::core::topology::Topology::Triangle>());
        s_localfactory->registerCreator("Quad", new DataCreator<sofa::core::topology::Topology::Quad>());
        s_localfactory->registerCreator("Tetra", new DataCreator<sofa::core::topology::Topology::Tetra>());
        s_localfactory->registerCreator("Hexa", new DataCreator<sofa::core::topology::Topology::Hexa>());
        s_localfactory->registerCreator("Penta", new DataCreator<sofa::core::topology::Topology::Penta>());

        // State vectors
        s_localfactory->registerCreator(
                    "Rigid3d::VecCoord", new DataCreator<sofa::defaulttype::Rigid3dTypes::VecCoord>());
        s_localfactory->registerCreator(
                    "Rigid3f::VecCoord", new DataCreator<sofa::defaulttype::Rigid3fTypes::VecCoord>());
        s_localfactory->registerCreator(
                    "Rigid3::VecCoord", new DataCreator<sofa::defaulttype::Rigid3Types::VecCoord>());

        // General vectors
        for (const auto& container : containers)
        {
            // Scalars
            s_localfactory->registerCreator(container + "<string>",
                                            new DataCreator<sofa::helper::vector<std::string>>());
            s_localfactory->registerCreator(container + "<float>",
                                            new DataCreator<sofa::helper::vector<float>>());
            s_localfactory->registerCreator(container + "<double>",
                                            new DataCreator<sofa::helper::vector<double>>());
            s_localfactory->registerCreator(container + "<bool>",
                                            new DataCreator<sofa::helper::vector<bool>>());
            s_localfactory->registerCreator(container + "<int>",
                                            new DataCreator<sofa::helper::vector<int>>());

            // vectors
            s_localfactory->registerCreator(
                        container + "<Vec2d>", new DataCreator<sofa::helper::vector<sofa::defaulttype::Vec2d>>());
            s_localfactory->registerCreator(
                        container + "<Vec3d>", new DataCreator<sofa::helper::vector<sofa::defaulttype::Vec3d>>());
            s_localfactory->registerCreator(
                        container + "<Vec4d>", new DataCreator<sofa::helper::vector<sofa::defaulttype::Vec4d>>());
            s_localfactory->registerCreator(
                        container + "<Vec6d>", new DataCreator<sofa::helper::vector<sofa::defaulttype::Vec6d>>());
            s_localfactory->registerCreator(
                        container + "<Vec2f>", new DataCreator<sofa::helper::vector<sofa::defaulttype::Vec2f>>());
            s_localfactory->registerCreator(
                        container + "<Vec3f>", new DataCreator<sofa::helper::vector<sofa::defaulttype::Vec3f>>());
            s_localfactory->registerCreator(
                        container + "<Vec4f>", new DataCreator<sofa::helper::vector<sofa::defaulttype::Vec4f>>());
            s_localfactory->registerCreator(
                        container + "<Vec6f>", new DataCreator<sofa::helper::vector<sofa::defaulttype::Vec6f>>());

            // Matrices
            s_localfactory->registerCreator(
                        container + "<Mat2x2d>",
                        new DataCreator<sofa::helper::vector<sofa::defaulttype::Mat2x2d>>());
            s_localfactory->registerCreator(
                        container + "<Mat3x3d>",
                        new DataCreator<sofa::helper::vector<sofa::defaulttype::Mat3x3d>>());
            s_localfactory->registerCreator(
                        container + "<Mat4x4d>",
                        new DataCreator<sofa::helper::vector<sofa::defaulttype::Mat4x4d>>());
            s_localfactory->registerCreator(
                        container + "<Mat2x2f>",
                        new DataCreator<sofa::helper::vector<sofa::defaulttype::Mat2x2f>>());
            s_localfactory->registerCreator(
                        container + "<Mat3x3f>",
                        new DataCreator<sofa::helper::vector<sofa::defaulttype::Mat3x3f>>());
            s_localfactory->registerCreator(
                        container + "<Mat4x4f>",
                        new DataCreator<sofa::helper::vector<sofa::defaulttype::Mat4x4f>>());

            // Topology
            s_localfactory->registerCreator(container + "<Edge>",
                                            new DataCreator<sofa::helper::vector<sofa::core::topology::Topology::Edge>>());
            s_localfactory->registerCreator(container + "<Triangle>",
                                            new DataCreator<sofa::helper::vector<sofa::core::topology::Topology::Triangle>>());
            s_localfactory->registerCreator(container + "<Quad>",
                                            new DataCreator<sofa::helper::vector<sofa::core::topology::Topology::Quad>>());
            s_localfactory->registerCreator(container + "<Tetra>",
                                            new DataCreator<sofa::helper::vector<sofa::core::topology::Topology::Tetra>>());
            s_localfactory->registerCreator(container + "<Hexa>",
                                            new DataCreator<sofa::helper::vector<sofa::core::topology::Topology::Hexa>>());
            s_localfactory->registerCreator(container + "<Penta>",
                                            new DataCreator<sofa::helper::vector<sofa::core::topology::Topology::Penta>>());
        }
    }
    return s_localfactory ;
}


}  // namespace sofapython3

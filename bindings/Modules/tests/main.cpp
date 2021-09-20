/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2021 INRIA, USTL, UJF, CNRS, MGH                     *
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

#include <SofaPython3/PythonTest.h>
#include <SofaPython3/PythonTestExtractor.h>
#include <sofa/helper/Utils.h>

#include <sofa/helper/logging/Messaging.h>
#include <sofa/core/logging/PerComponentLoggingMessageHandler.h>
#include <sofa/helper/logging/MessageDispatcher.h>
using sofa::helper::logging::MessageDispatcher;


/// static build of the test list
static struct Tests : public sofapython3::PythonTestExtractor
{
    Tests() {
        using sofa::helper::logging::MessageDispatcher;
        using sofa::helper::logging::MainPerComponentLoggingMessageHandler;

        MessageDispatcher::addHandler(&MainPerComponentLoggingMessageHandler::getInstance()) ;

        const std::string executable_directory = sofa::helper::Utils::getExecutableDirectory();
        addTestDirectory(executable_directory+"/Bindings.Modules.Tests/SofaBaseTopology", "SofaBaseTopology_");
        addTestDirectory(executable_directory+"/Bindings.Modules.Tests/SofaDeformable", "SofaDeformable_");
    }
} python_tests;

/// run test list using the custom name function getTestName.
/// this allows to do gtest_filter=*FileName*
class Modules : public sofapython3::PythonTest {};
INSTANTIATE_TEST_SUITE_P(SofaPython3,
                        Modules,
                        ::testing::ValuesIn(python_tests.extract()),
                        Modules::getTestName);

TEST_P(Modules, all_tests) { run(GetParam()); }

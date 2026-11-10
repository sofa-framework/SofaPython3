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

#include <SofaPython3Testing/PythonTest.h>
#include <SofaPython3Testing/PythonTestExtractor.h>
#include <sofa/helper/Utils.h>
#include <sofa/helper/system/FileSystem.h>
#include <algorithm>

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
        // One directory per bound SOFA module, only present if the module bindings were built
        for (const std::string module : {"Constraint/Lagrangian/Solver", "LinearSolver/Iterative", "SolidMechanics/Spring"})
        {
            const std::string directory = executable_directory + "/Bindings.Component.Tests.d/" + module;
            if (sofa::helper::system::FileSystem::exists(directory))
            {
                std::string testgroup = module + "_";
                std::replace(testgroup.begin(), testgroup.end(), '/', '_');
                addTestDirectory(directory, testgroup);
            }
        }
    }
} python_tests;

/// run test list using the custom name function getTestName.
/// this allows to do gtest_filter=*FileName*
class Component : public sofapython3::PythonTest {};
INSTANTIATE_TEST_SUITE_P(SofaPython3,
                        Component,
                        ::testing::ValuesIn(python_tests.extract()),
                        Component::getTestName);

TEST_P(Component, all_tests) { run(GetParam()); }

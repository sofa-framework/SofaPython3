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

#include <vector>

#include <SofaPython3Testing/PythonTest.h>
#include <sofa/helper/Utils.h>
#include <SofaPython3Testing/PythonTestExtractor.h>

using sofapython3::PythonTest ;
using sofapython3::PythonTestExtractor ;
using sofapython3::PrintTo ;
using std::string;

#include <sofa/helper/logging/Messaging.h>
#include <sofa/core/logging/PerComponentLoggingMessageHandler.h>
#include <sofa/helper/logging/MessageDispatcher.h>
using sofa::helper::logging::MessageDispatcher;
using sofa::helper::logging::MainPerComponentLoggingMessageHandler;

namespace
{

bool init()
{
    MessageDispatcher::addHandler(&MainPerComponentLoggingMessageHandler::getInstance()) ;
    return true;
}

static int _inited_=init();

class Sofa : public PythonTest {};

/// static build of the test list
static struct PythonModule_Sofa_tests : public PythonTestExtractor
{
    PythonModule_Sofa_tests()
    {
        const std::string executable_directory = sofa::helper::Utils::getExecutableDirectory();
        addTestDirectory(executable_directory+"/Bindings.Sofa.Tests.d/Core", "Sofa_Core_");
        addTestDirectory(executable_directory+"/Bindings.Sofa.Tests.d/Helper", "Sofa_Helper_");
        addTestDirectory(executable_directory+"/Bindings.Sofa.Tests.d/Simulation", "Sofa_Simulation_");
        addTestDirectory(executable_directory+"/Bindings.Sofa.Tests.d/Types", "Sofa_Types_");
        addTestDirectory(executable_directory+"/Bindings.Sofa.Tests.d/Components", "Sofa_Components_");
    }
} python_tests;

/// run test list using the custom name function getTestName.
/// this allows to do gtest_filter=*FileName*
INSTANTIATE_TEST_SUITE_P(SofaPython3,
                        Sofa,
                        ::testing::ValuesIn(python_tests.extract()),
                        Sofa::getTestName);

TEST_P(Sofa, all_tests) { run(GetParam()); }

}

/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2018 INRIA, USTL, UJF, CNRS, MGH                    *
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
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
/******************************************************************************
 * Contributors:                                                              *
 *   - damien.marchal@univ-lille.fr                                           *
 *   - bruno.josue.marques@inria.fr                                           *
 *   - eve.le-guillou@centrale.centralelille.fr                               *
 *   - jean-nicolas.brunet@inria.fr                                           *
 *   - thierry.gaugry@inria.fr                                                *
 *   - damien.marchal@univ-lille1.fr                                          *
 *****************************************************************************/

#include <vector>

#include <SofaPython3/PythonTest.h>
#include <sofa/helper/Utils.h>
#include <SofaPython3/PythonTestExtractor.h>

using sofapython3::PythonTest ;
using sofapython3::PythonTestExtractor ;
using sofapython3::PrintTo ;
using std::string;

#include <sofa/core/logging/PerComponentLoggingMessageHandler.h>
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
        addTestDirectory(executable_directory+"/Components", "Sofa_Components_");
        addTestDirectory(executable_directory+"/Core", "Sofa_Core_");
        addTestDirectory(executable_directory+"/Helper", "Sofa_Helper_");
        addTestDirectory(executable_directory+"/Simulation", "Sofa_Simulation_");
        addTestDirectory(executable_directory+"/Types", "Sofa_Types_");
    }
} python_tests;

/// run test list using the custom name function getTestName.
/// this allows to do gtest_filter=*FileName*
INSTANTIATE_TEST_CASE_P(SofaPython3,
                        Sofa,
                        ::testing::ValuesIn(python_tests.extract()),
                        Sofa::getTestName);

TEST_P(Sofa, all_tests) { run(GetParam()); }

}

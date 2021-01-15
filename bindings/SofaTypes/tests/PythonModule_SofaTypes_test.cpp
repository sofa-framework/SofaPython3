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

#include <vector>

#include <SofaPython3/PythonTest.h>
#include <sofa/helper/Utils.h>
#include <SofaPython3/PythonTestExtractor.h>

using sofapython3::PythonTest ;
using sofapython3::PythonTestExtractor ;
using sofapython3::PrintTo ;
using std::string;

namespace
{

  class PythonModule_SofaTypes_test : public PythonTestExtractor
  {
  public:
    PythonModule_SofaTypes_test()
    {
        const std::string executable_directory = sofa::helper::Utils::getExecutableDirectory();
        addTestDirectory(executable_directory+"/pyfiles", "SofaTypes_");
    }
  } python_tests;

  /// run test list using the custom name function getTestName.
  /// this allows to do gtest_filter=*FileName*
  INSTANTIATE_TEST_CASE_P(Batch,
			  PythonTest,
              ::testing::ValuesIn(python_tests.extract()),
              PythonTest::getTestName);

  TEST_P(PythonTest, all_tests) { run(GetParam()); }

}

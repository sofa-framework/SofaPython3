/*********************************************************************
Copyright 2019, CNRS, University of Lille, INRIA

This file is part of sofaPython3

sofaPython3 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

sofaPython3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with sofaqtquick. If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/
/********************************************************************
 Contributors:
    - damien.marchal@univ-lille.fr
    - bruno.josue.marques@inria.fr
    - eve.le-guillou@centrale.centralelille.fr
    - jean-nicolas.brunet@inria.fr
    - thierry.gaugry@inria.fr
********************************************************************/

#pragma once

#include <string>
#include <SofaPython3/config.h>
#include <sofa/helper/testing/BaseTest.h>

#include <boost/filesystem/path.hpp>
using boost::filesystem::path;

namespace sofapython3
{

using sofa::helper::testing::BaseTest;

/// a Python_test is defined by a python filepath and optional arguments
struct SOFAPYTHON3_API PythonTestData
{
    PythonTestData (std::string filepath, std::string testgroup, std::vector<std::string> arguments)
            : filepath(std::move(filepath)), arguments(std::move(arguments)), testgroup{std::move(testgroup)} {}

    std::string filepath;
    std::vector<std::string> arguments;
    std::string testgroup;
};

/// This function is used by gtest to print the content of the struct in a human friendly way
/// eg:
///        test.all_tests/2, where GetParam() = /path/to/file.py with args {1,2,3}
/// instead of the defautl googletest printer that output things like the following:
///        test.all_tests/2, where GetParam() = 56-byte object <10-48 EC-37 18-56 00-00 67-00-00-00>
void SOFAPYTHON3_API PrintTo(const PythonTestData& d, ::std::ostream* os);

/// A test written in python (but not as a sofa class to perform unitary testing on python functions)
class SOFAPYTHON3_API PythonTest : public BaseTest,
        public ::testing::WithParamInterface<PythonTestData>
{
public:
    void run( const PythonTestData& );

    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    static void SetUpTestCase();

    // Per-test-suite tear-down.
    // Called after the last test in this test suite.
    static void TearDownTestCase();

    /// This function is called by gtest to generate the test from the filename. This is nice
    /// As this allows to do mytest --gtest_filter=*MySomething*
    static std::string getTestName(const testing::TestParamInfo<PythonTestData>& p)
    {
        if(p.param.arguments.size()==0)
            return  std::to_string(p.index)+"_"+p.param.testgroup+path(p.param.filepath).stem().string();
        return  std::to_string(p.index)+"_"+p.param.testgroup+path(p.param.filepath).stem().string()
                                       +"_"+p.param.arguments[0];
    }
};

}

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

#include <fstream>

#include <pybind11/pybind11.h>
using namespace pybind11::literals; // to bring in the `_a` literal
#include <pybind11/embed.h>
namespace py = pybind11;

#include <sofa/testing/BaseTest.h>
#include <sofa/helper/logging/Messaging.h>
#include <sofa/helper/system/FileSystem.h>

#include <sofa/helper/system/PluginManager.h>
using sofa::helper::system::PluginManager;

#include <sofa/helper/system/SetDirectory.h>
using sofa::helper::system::SetDirectory;

#include <SofaPython3Testing/PythonTestExtractor.h>
#include <numeric>
#include <SofaPython3/PythonEnvironment.h>
#include "PythonTest.h"

MSG_REGISTER_CLASS(sofapython3::PythonTest, "SofaPython3::PythonTest")

namespace sofapython3
{

/// This function is used by gtest to print the content of the struct in a meaninfull way
void PrintTo(const sofapython3::PythonTestData& d, ::std::ostream *os)
{
    (*os) << d.filepath  ;
    (*os) << " with args {" ;
    for(auto& v : d.arguments)
    {
        (*os) << v << ", " ;
    }
    (*os) << "}";
}


///////////////////////// PythonTest Definition  //////////////////////////////////////////////////

bool runTests(py::module& unittest, py::object suite)
{
    // Save the current python's stdout and stderr to temporary variables
    py::object sys = py::module::import("sys");
    py::object py_stdout = sys.attr("stdout");
    py::object py_stderr = sys.attr("stderr");

    // Set up new string buffer
    py::object io = py::module::import("io");
    py::object string_buffer = io.attr("StringIO")();

    // Use these buffers for python's stdout and stderr
    sys.attr("stdout") = string_buffer;
    sys.attr("stderr") = string_buffer;

    // Set up the test runner
    py::dict kwargs = py::dict("verbosity"_a=0);
    py::object testRunner = unittest.attr("TextTestRunner")(**kwargs);

    // Run the unittests
    py::object testSuiteResults = testRunner.attr("run")(suite);

    // Flush the buffers to c++ stdout and stderr
    sys.attr("stdout").attr("flush")();
    sys.attr("stderr").attr("flush")();
    std::cout << (std::string) py::str(string_buffer.attr("getvalue")()) << std::flush;

    // Swap back the original python's stdout and stderr
    sys.attr("stdout") = py_stdout;
    sys.attr("stderr") = py_stderr;

    // Return the result of the testsuite
    return py::cast<bool>(testSuiteResults.attr("wasSuccessful")());
}

void PythonTest::run( const PythonTestData& data )
{
    // Flush buffers
    std::cout << std::flush;
    std::cerr << std::flush;

    {
        EXPECT_MSG_NOEMIT(Error);

        try{
            py::module unittest = py::module::import("unittest");
            py::object globals = py::module::import("__main__").attr("__dict__");
            py::module module;

            const char* filename = data.filepath.c_str();
            SetDirectory localDir(filename);
            std::string basename = SetDirectory::GetFileNameWithoutExtension(SetDirectory::GetFileName(filename).c_str());
            module = PythonEnvironment::importFromFile(basename, SetDirectory::GetFileName(filename),
                                                       &globals);

            py::object testSuite = PythonTestExtractor::getTestSuite(unittest, module, data.arguments);
            py::list testSuiteList = py::cast<py::list>(testSuite);
            if(!testSuiteList.size())
            {
                msg_warning() << "There doesn't seem to be any test in " << filename;
                return ;
            }

            if(!runTests(unittest, testSuite))
            {
                ADD_FAILURE_AT(data.filepath.c_str(), 0);
            }
        } catch (std::exception& e) {
            msg_error() << e.what();
            ADD_FAILURE_AT(data.filepath.c_str(), 0);
        } catch (...) {
            ADD_FAILURE_AT(data.filepath.c_str(), 0);
        }
    }

    // Flush buffers
    std::cout << std::flush;
    std::cerr << std::flush;

    //PythonEnvironment::Release();
}

static PythonEnvironment::gil * test_gil = nullptr;

void PythonTest::SetUpTestCase ()
{
    // The following will be executed once before the first python test file
    PythonEnvironment::Init();
    test_gil = new PythonEnvironment::gil;
    py::module::import("SofaRuntime");
    py::module::import("Sofa");
}

void PythonTest::TearDownTestCase ()
{
    // The following will be executed once after the last python test file
    delete test_gil;
    test_gil = nullptr;
    // Seg fault...
    // PythonEnvironment::Release();
}

} /// namespace sofapython3

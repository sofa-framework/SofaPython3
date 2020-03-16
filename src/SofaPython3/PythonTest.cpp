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
#include <fstream>

#include <pybind11/pybind11.h>
using namespace pybind11::literals; // to bring in the `_a` literal
#include <pybind11/embed.h>
namespace py = pybind11;

#include <sofa/helper/testing/BaseTest.h>
#include <sofa/helper/logging/Messaging.h>
#include <sofa/helper/system/FileSystem.h>

#include <sofa/helper/system/PluginManager.h>
using sofa::helper::system::PluginManager;

#include <SofaSimulationGraph/SimpleApi.h>
namespace simpleapi=sofa::simpleapi;

#include <sofa/helper/system/SetDirectory.h>
using sofa::helper::system::SetDirectory;

#include "PythonEnvironment.h"
#include "PythonTest.h"

MSG_REGISTER_CLASS(sofapython3::PythonTest, "SofaPython3::PythonTest")

namespace sofapython3
{

/// This function is used by gtest to print the content of the struct in a meaninfull way
void SOFAPYTHON3_API PrintTo(const sofapython3::PythonTestData& d, ::std::ostream *os)
{
    (*os) << d.filepath  ;
    (*os) << " with args {" ;
    for(auto& v : d.arguments)
    {
        (*os) << v << ", " ;
    }
    (*os) << "}";
}

///////////////////////// PythonTestData Definition  ///////////////////////////////////////////////
PythonTestData::PythonTestData(const std::string& filepath, const std::string &testgroup, const std::vector<std::string>& arguments ) :
    filepath(filepath), arguments(arguments), testgroup{testgroup} {}


///////////////////////// PythonTest Definition  //////////////////////////////////////////////////
PythonTest::PythonTest()
{

}

PythonTest::~PythonTest()
{
}


// extract the test suite from a module
py::object getTestSuite(py::module& unittest, py::module& module, const std::vector<std::string>& arguments)
{
    py::object testSuite = py::list();
    py::module inspect = py::module::import("inspect");

    py::list classes = inspect.attr("getmembers")(module);
    for(const auto n : classes)
    {
        std::string name = py::cast<std::string>(py::cast<py::tuple>(n)[0]);
        py::object obj = py::cast<py::tuple>(n)[1];
        if (py::cast<bool>(inspect.attr("isclass")(obj)) == true)
        {
            py::tuple bases = obj.attr("__bases__");
            for (const auto base : bases)
            {
                if (py::cast<std::string>(base.attr("__name__")) == "TestCase")
                {
                    if (arguments.empty())
                        return unittest.attr("TestLoader")().attr("loadTestsFromTestCase")(obj);

                    testSuite = unittest.attr("TestSuite")();
                    py::list list;
                    for (auto& arg : arguments) {
                        list.append(py::str(arg));
                    }
                    testSuite.attr("addTest")(obj(*list));
                }
            }
        }
    }
    return testSuite;
}

bool runTests(py::module& unittest, py::object suite)
{
    py::dict kwargs = py::dict("verbosity"_a=2);
    py::object testRunner = unittest.attr("TextTestRunner")(**kwargs);

    py::object testSuiteResults = testRunner.attr("run")(suite);
    return py::cast<bool>(testSuiteResults.attr("wasSuccessful")());
}

void PythonTest::run( const PythonTestData& data )
{
    msg_info() << "running " << data.filepath;

    PythonEnvironment::Init();
    {
        EXPECT_MSG_NOEMIT(Error);
        simpleapi::importPlugin("SofaComponentAll");
        sofa::simulation::setSimulation(simpleapi::createSimulation().get());

        try{
            PythonEnvironment::gil scoped_gil;

            py::module::import("Sofa");
            py::module unittest = py::module::import("unittest");
            py::object globals = py::module::import("__main__").attr("__dict__");
            py::module module;

            const char* filename = data.filepath.c_str();
            SetDirectory localDir(filename);
            std::string basename = SetDirectory::GetFileNameWithoutExtension(SetDirectory::GetFileName(filename).c_str());
            module = PythonEnvironment::importFromFile(basename, SetDirectory::GetFileName(filename),
                                                       globals);

            py::object testSuite = getTestSuite(unittest, module, data.arguments);
            py::list testSuiteList = py::cast<py::list>(testSuite);
            if(!testSuiteList.size())
            {
                msg_error() << "There doesn't seem to be any test in " << filename;
                return ;
            }

            if(!runTests(unittest, testSuite))
            {
                FAIL();
            }
        }catch(std::exception& e)
        {
            msg_error() << e.what();
            FAIL();
        }catch(...)
        {
            FAIL();
        }
    }

    //PythonEnvironment::Release();
}

/// add a Python_test_data with given path
void PythonTestList::addTest( const std::string& filename,
                              const std::string& path,
                              const std::string& testgroup,
                              const std::vector<std::string>& arguments
                              )
{
    PythonEnvironment::Init();
    PythonEnvironment::gil scoped_gil;
    try {
        py::module::import("Sofa");
        py::module unittest = py::module::import("unittest");
        py::object globals = py::module::import("__main__").attr("__dict__");
        py::module module;

        const char* filenameC = filename.c_str();
        std::string fullpath = (path+"/"+filename);
        const char* pathC = fullpath.c_str();

        SetDirectory localDir(pathC);
        std::string basename = SetDirectory::GetFileNameWithoutExtension(SetDirectory::GetFileName(filenameC).c_str());
        module = PythonEnvironment::importFromFile(basename, SetDirectory::GetFileName(filenameC),
                                                   globals);
        std::list<std::string> testNames;
        py::list testSuite = getTestSuite(unittest, module, arguments);
        if(!testSuite.size())
        {
            throw std::runtime_error("No test suite found. Make sure there is at least one class in the script that inherits from TestCase.");
        }
        for (const auto test : testSuite)
            testNames.push_back(py::cast<std::string>(test.attr("id")()));


        for(const auto& test : testNames)
        {
            std::vector<std::string> cargs;
            cargs.push_back(test.substr(test.find_last_of('.')+1));
            cargs.insert(cargs.end(), arguments.begin(), arguments.end());
            list.emplace_back( PythonTestData( filepath(path,filename), testgroup, cargs ) );
        }
    } catch(const std::exception& e) {
        msg_error("PythonTestList") << "File skipped: " << (path+"/"+filename) << msgendl
                                    << e.what();
    }
}

void PythonTestList::addTestDir(const std::string& dir, const std::string& testgroup, const std::string& prefix)
{
    std::vector<std::string> files;
    sofa::helper::system::FileSystem::listDirectory(dir, files);

    for(const std::string& file : files)
    {
        if( sofa::helper::starts_with(prefix, file)
                && (sofa::helper::ends_with(".py", file) || sofa::helper::ends_with(".py3", file)))
        {
            addTest(file, dir, testgroup);
        }
    }
}

} /// namespace sofapython3

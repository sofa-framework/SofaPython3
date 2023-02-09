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

#include <SofaPython3Testing/PythonTestExtractor.h>
#include <SofaPython3/PythonEnvironment.h>

#include <sofa/helper/logging/Messaging.h>
#include <sofa/helper/system/FileSystem.h>
#include <sofa/helper/StringUtils.h>
#include <sofa/helper/system/SetDirectory.h>

#include <sofa/helper/system/PluginManager.h>
using sofa::helper::system::PluginManager;

#include <sofa/helper/logging/Messaging.h>

using sofa::helper::system::SetDirectory;
namespace py = pybind11;

#include <list>

namespace sofapython3 {
// extract the test suite from a module
py::object PythonTestExtractor::getTestSuite(py::module& unittest, py::module& module, const std::vector<std::string>& arguments)
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

std::vector<PythonTestData> PythonTestExtractor::extract () const
{
    PluginManager::getInstance().loadPlugin("SofaPython3");

    PythonEnvironment::Init();
    PythonEnvironment::gil scoped_gil;

    py::module unittest = py::module::import("unittest");
    py::module::import("SofaRuntime");
    py::module::import("Sofa");
    py::object globals = py::module::import("__main__").attr("__dict__");

    std::vector<PythonTestData> list;

    for (const auto & test : p_tests) {
        std::string fullpath = (test.path + "/" + test.filename);
        SetDirectory localDir(fullpath.c_str());
        std::string basename = SetDirectory::GetFileNameWithoutExtension(
            SetDirectory::GetFileName(test.filename.c_str()).c_str()
        );

        try {
            py::module module = PythonEnvironment::importFromFile(
                basename, SetDirectory::GetFileName(test.filename.c_str()), &globals
            );

            std::list<std::string> testNames;
            py::list testSuite = getTestSuite(unittest, module, test.arguments);
            if(!testSuite.size())
            {
                throw std::runtime_error("No test suite found. Make sure there is at least one class in "
                                         "the script that inherits from TestCase.");
            }
            for (const auto t : testSuite) {
                testNames.push_back(py::cast<std::string>(t.attr("id")()));
            }

            for(const auto& test_name : testNames) {
                std::vector<std::string> cargs;
                cargs.push_back(test_name.substr(test_name.find_last_of('.')+1));
                cargs.insert(cargs.end(), test.arguments.begin(), test.arguments.end());
                list.emplace_back( PythonTestData( filepath(test.path, test.filename), test.testgroup, cargs ) );
            }
            msg_info("PythonTestExtractor") << "File '" << test.filename << "' loaded with " << testNames.size() << " unit tests.";

        } catch(const std::exception& e) {
            msg_error("PythonTestExtractor") << "File skipped: " << (test.path+"/"+test.filename) << msgendl
                                        << e.what();
        }
    }

    // Seg fault...
    // PythonEnvironment::Release();
    return list;
}

void PythonTestExtractor::addTestFile (const std::string & filename,
                                       const std::string & path,
                                       const std::string & testgroup,
                                       const std::vector<std::string> & arguments)
{
    p_tests.push_back({filename, path, testgroup, arguments});
}

void PythonTestExtractor::addTestDirectory (const std::string & dir, const std::string & testgroup, const std::string & prefix)
{
    std::vector<std::string> files;
    sofa::helper::system::FileSystem::listDirectory(dir, files);
    for(const std::string& file : files) {
        if( sofa::helper::starts_with(prefix, file)
            && (sofa::helper::ends_with(".py", file) || sofa::helper::ends_with(".py3", file)))
        {
            addTestFile(file, dir, testgroup);
        }
    }
}

} // namespace sofapython3

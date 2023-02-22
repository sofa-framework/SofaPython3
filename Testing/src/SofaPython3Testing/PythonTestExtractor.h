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
#pragma once
#include <SofaPython3Testing/config.h>
#include <SofaPython3Testing/PythonTest.h>

#include <utility>
#include <pybind11/pybind11.h>

#pragma once

namespace sofapython3
{

/**
 * Utility class that loads up python files and extract the unittest.
 */
class SOFAPYTHON3_TESTING_API PythonTestExtractor
{
public:
    /**
     * Extract the list of python tests from every files added through addTest() and addTestDir().
     */
    [[nodiscard]]
    std::vector<PythonTestData> extract () const;

    static pybind11::object getTestSuite(pybind11::module& unittest, pybind11::module& module, const std::vector<std::string>& arguments);

protected:
    /// add a Python_test_data with given path
    void addTestFile (const std::string &filename,
                      const std::string &path = "", const std::string &testgroup = "",
                      const std::vector<std::string> &arguments = std::vector<std::string>(0));

    /// add all the python test files in `dir` starting with `prefix`
    void addTestDirectory (const std::string &dir, const std::string &testgroup = "", const std::string &prefix = "");

private:
/// concatenate path and filename
    static std::string filepath (const std::string &path, const std::string &filename)
    {
        if (path != "")
            return path + "/" + filename;
        else
            return filename;
    }

    struct Entry {
        std::string filename;
        std::string path;
        std::string testgroup;
        std::vector<std::string> arguments;
    };

    std::vector<Entry> p_tests;
};

} // namespace sofapython3

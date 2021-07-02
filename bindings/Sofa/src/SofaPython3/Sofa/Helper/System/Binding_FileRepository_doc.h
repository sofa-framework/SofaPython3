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

namespace sofapython3::doc::FileRepository
{
static auto Class =
R"(
Helper class to find files in a list of directories.

Each file is searched as follow:

1: Using the specified filename in current directory, or in the specified directory.
If the filename does not start with "/", "./", or "../" :
2: In the directory path specified using addFirstPath method.
3: In the directory path specified using an environment variable (default to SOFA_DATA_PATH).
4: In the default directories relative to the main executable (default to ../share).
5: In the directory path specified using addLastPath method.

For file name starting with '/', './' or '../' only the first step is used.
A path is considered as a concatenation of directories separated by : on linux / mac and ; on windows

)";

static auto ClassConstructor =
R"(
FileRepository(envVar = 'SOFA_DATA_PATH', relativePath = None, iniFilePath = None)
:param envVar: Every directory paths found in this environment variable will be automatically added to this file repository
:param relativePath:
:param iniFilePath: Every directory paths defined in this INI file will be added to this file repository
)";

static auto addFirstPath =
R"(
Adds a path to the front of the set of paths.
)";

static auto addLastPath =
R"(
Adds a path to the back of the set of paths.
)";

static auto getFirstPath =
R"(
Get the first path into the set of paths.
)";

static auto getPaths =
R"(
Get the list of directory paths included in this file repository.
)";

static auto removePath =
R"(
Remove a path of the set of paths.
)";

static auto clear =
R"(
Remove all known paths.
)";

static auto relativeToPath =
R"(
Returns a string such as refPath + string = path if path contains refPath.
Otherwise returns path.
)";

static auto getPathsJoined =
R"(
Get the list of directory paths included in this file repository into a string separated by Sofa.Helper.System.FileRepository.entrySeparator().
)";

static auto directAccessProtocolPrefix =
R"(
A protocol like http: or file: which will bypass the file search if found in the filename of the findFile* functions that directly returns the path as if the function succeeded
Use case: add the prefix ram: as the direct protocol, this way the FileRepository will not try to look for the file on the hard disk and will directly return
then the inherited FileAccess singleton enhanced with the capacity to find ram file will deliver a correct stream to this in-ram virtual file
)";

static auto findFile =
R"(
Find file using the stored set of paths.
:param basefile: override current directory by using the parent directory of the given file
:param filename: requested file as input, resolved file path as output
:return: true if the file was found in one of the directories, false otherwise
)";

static auto getFile =
R"(
Alias for findFile, but returning the resolved file as the result. Less informative for errors, but sometimes easier to use.
)";

static auto findFileFromFile =
R"(
Find file using the stored set of paths.
:param basefile: override current directory by using the parent directory of the given file
:param filename: requested file as input, resolved file path as output
:return: true if the file was found in one of the directories, false otherwise
)";

static auto print =
R"(
Print the list of path to std::cout.
)";

static auto entrySeparator =
R"(
OS-dependant character separing entries in list of paths.
)";

} // namespace sofapython3::doc::FileRepository
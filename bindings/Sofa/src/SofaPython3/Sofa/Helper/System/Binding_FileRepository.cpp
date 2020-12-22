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

#include <pybind11/stl.h>
#include <SofaPython3/Sofa/Helper/System/Binding_FileRepository.h>
#include <SofaPython3/Sofa/Helper/System/Binding_FileRepository_doc.h>
#include <sofa/helper/system/FileRepository.h>

namespace py { using namespace pybind11; }


namespace sofapython3 {

void moduleAddFileRepository(py::module &m) {
    py::class_<sofa::helper::system::FileRepository> file_repository (m, "FileRepository");
    file_repository.doc() = doc::FileRepository::Class;

    file_repository.def(
        py::init<const char*, const char*, const sofa::helper::system::FileRepository::fileKeysMap& >(),
        py::arg("envVar") = "SOFA_DATA_PATH",
        py::arg("relativePath") = (const char *) nullptr,
		py::arg("fileKeyMaps") = sofa::helper::system::FileRepository::fileKeysMap{},
        doc::FileRepository::ClassConstructor
    );

    file_repository.def(
        "addFirstPath",
        &sofa::helper::system::FileRepository::addFirstPath,
        py::arg("path").none(false),
        doc::FileRepository::addFirstPath
    );

    file_repository.def(
        "addLastPath",
        &sofa::helper::system::FileRepository::addLastPath,
        py::arg("path").none(false),
        doc::FileRepository::addLastPath
    );

    file_repository.def(
        "getFirstPath",
        &sofa::helper::system::FileRepository::getFirstPath,
        doc::FileRepository::getFirstPath
    );

    file_repository.def(
        "getPaths",
        &sofa::helper::system::FileRepository::getPaths,
        doc::FileRepository::getPaths
    );

    file_repository.def(
        "removePath",
        &sofa::helper::system::FileRepository::removePath,
        py::arg("path").none(false),
        doc::FileRepository::removePath
    );

    file_repository.def(
        "clear",
        &sofa::helper::system::FileRepository::clear,
        doc::FileRepository::clear
    );

    file_repository.def_static(
        "relativeToPath",
        &sofa::helper::system::FileRepository::relativeToPath,
        py::arg("path").none(false), py::arg("refPath").none(false), py::arg("doLowerCaseOnWin32") = true,
        doc::FileRepository::relativeToPath
    );

    file_repository.def(
        "getPathsJoined",
        &sofa::helper::system::FileRepository::getPathsJoined,
        doc::FileRepository::getPathsJoined
    );

    file_repository.def_property(
        "direct_access_protocol_prefix",
        &sofa::helper::system::FileRepository::getDirectAccessProtocolPrefix,
        &sofa::helper::system::FileRepository::setDirectAccessProtocolPrefix,
        doc::FileRepository::directAccessProtocolPrefix
    );

    file_repository.def(
        "findFile",
        [](sofa::helper::system::FileRepository & self, std::string & filename, const std::string & basedir = "") -> bool {
            return self.findFile(filename, basedir);
        },
        py::arg("filename").none(false), py::arg("basedir").none(true),
        doc::FileRepository::findFile
    );

    file_repository.def(
        "findFile",
        [](sofa::helper::system::FileRepository & self, std::string & filename) -> bool {
            return self.findFile(filename);
        },
        py::arg("filename").none(false),
        doc::FileRepository::findFile
    );

    file_repository.def(
        "getFile",
        [](sofa::helper::system::FileRepository & self, std::string & filename, const std::string & basedir = "") -> std::string {
            return self.getFile(filename, basedir);
        },
        py::arg("filename").none(false), py::arg("basedir").none(true),
        doc::FileRepository::getFile
    );

    file_repository.def(
        "getFile",
        [](sofa::helper::system::FileRepository & self, std::string & filename) -> std::string {
            return self.getFile(filename);
        },
        py::arg("filename").none(false),
        doc::FileRepository::getFile
    );

    file_repository.def(
        "findFileFromFile",
        [](sofa::helper::system::FileRepository & self, std::string & filename, const std::string & basefile = "") -> bool {
            return self.findFileFromFile(filename, basefile);
        },
        py::arg("filename").none(false), py::arg("basefile").none(false),
        doc::FileRepository::findFileFromFile
    );

    file_repository.def(
        "print",
        &sofa::helper::system::FileRepository::print,
        doc::FileRepository::print
    );

    file_repository.def_static(
        "entrySeparator",
        &sofa::helper::system::FileRepository::entrySeparator,
        doc::FileRepository::entrySeparator
    );

}

} /// namespace sofapython3

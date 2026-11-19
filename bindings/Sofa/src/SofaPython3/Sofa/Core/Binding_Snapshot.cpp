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

#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl_bind.h>
#include <SofaPython3/Sofa/Core/Binding_Snapshot.h>
#include <SofaPython3/PythonFactory.h>

using sofa::core::objectmodel::Snapshot;

namespace py { using namespace pybind11; }
PYBIND11_MAKE_OPAQUE(std::vector<sofa::core::objectmodel::Snapshot::DataInfo>);
PYBIND11_MAKE_OPAQUE(std::vector<sofa::core::objectmodel::Snapshot::LinkInfo>);
PYBIND11_MAKE_OPAQUE(std::vector<std::shared_ptr<sofa::core::objectmodel::Snapshot::SnapshotObject>>);
PYBIND11_MAKE_OPAQUE(std::vector<std::shared_ptr<sofa::core::objectmodel::Snapshot::SnapshotNode>>);

namespace sofapython3 {

void moduleAddSnapshot(py::module &m)
{
    py::bind_vector<std::vector<Snapshot::DataInfo>>(m, "SnapshotDataInfoVector");
    py::bind_vector<std::vector<Snapshot::LinkInfo>>(m, "SnapshotLinkInfoVector");
    py::bind_vector<std::vector<std::shared_ptr<Snapshot::SnapshotObject>>>(m, "SnapshotObjectVector");
    py::bind_vector<std::vector<std::shared_ptr<Snapshot::SnapshotNode>>>(m, "SnapshotNodeVector");

    py::class_<Snapshot> snapshot(m, "Snapshot");

    snapshot.def(py::init<>())
            .def_readwrite("graphRoot", &Snapshot::m_graphRoot);

    py::class_<Snapshot::DataInfo>(snapshot, "DataInfo")
        .def(py::init<>())
        .def_readwrite("name", &Snapshot::DataInfo::m_name)
        .def_readwrite("type", &Snapshot::DataInfo::m_type)
        .def_readwrite("value", &Snapshot::DataInfo::m_value);

    py::class_<Snapshot::LinkInfo>(snapshot, "LinkInfo")
        .def(py::init<>())
        .def_readwrite("name", &Snapshot::LinkInfo::m_name)
        .def_readwrite("type", &Snapshot::LinkInfo::m_type)
        .def_readwrite("value", &Snapshot::LinkInfo::m_value);

    py::class_<Snapshot::SnapshotObject, std::shared_ptr<Snapshot::SnapshotObject>>(snapshot, "SnapshotObject")
        .def(py::init<>())
        .def(py::init<std::string>())
        .def_readwrite("name", &Snapshot::SnapshotObject::m_name)
        .def_readwrite("className", &Snapshot::SnapshotObject::m_className)
        .def_readwrite("pathName", &Snapshot::SnapshotObject::m_pathName)

        .def_property("dataContainer",
            [](Snapshot::SnapshotObject &self) -> std::vector<Snapshot::DataInfo>& { return self.m_dataContainer; },
            [](Snapshot::SnapshotObject &self, const std::vector<Snapshot::DataInfo> &v) { self.m_dataContainer = v; },
            py::return_value_policy::reference_internal)

        .def_property("linkContainer",
            [](Snapshot::SnapshotObject &self) -> std::vector<Snapshot::LinkInfo>& { return self.m_linkContainer; },
            [](Snapshot::SnapshotObject &self, const std::vector<Snapshot::LinkInfo> &v) { self.m_linkContainer = v; },
            py::return_value_policy::reference_internal)

        .def_property("objects",
            [](Snapshot::SnapshotObject &self) -> std::vector<std::shared_ptr<Snapshot::SnapshotObject>>& { return self.m_objects; },
            [](Snapshot::SnapshotObject &self, const std::vector<std::shared_ptr<Snapshot::SnapshotObject>> &v) { self.m_objects = v; },
            py::return_value_policy::reference_internal)

        .def("clear", &Snapshot::SnapshotObject::clear)

        .def("push_back", py::overload_cast<const Snapshot::DataInfo&>(&Snapshot::SnapshotObject::push_back), py::return_value_policy::reference_internal)
        .def("push_back", py::overload_cast<const Snapshot::LinkInfo&>(&Snapshot::SnapshotObject::push_back), py::return_value_policy::reference_internal)
        .def("push_back", py::overload_cast<const std::shared_ptr<Snapshot::SnapshotObject>&>(&Snapshot::SnapshotObject::push_back), py::return_value_policy::reference_internal);

    py::class_<Snapshot::SnapshotNode, Snapshot::SnapshotObject, std::shared_ptr<Snapshot::SnapshotNode>>(snapshot, "SnapshotNode")
        .def(py::init<>())
        .def(py::init<const std::string&>())
        .def(py::init<const Snapshot::SnapshotObject&>())

        .def_property("children",
            [](Snapshot::SnapshotNode &self) -> std::vector<std::shared_ptr<Snapshot::SnapshotNode>>& { return self.m_children; },
            [](Snapshot::SnapshotNode &self, const std::vector<std::shared_ptr<Snapshot::SnapshotNode>> &v) { self.m_children = v; },
            py::return_value_policy::reference_internal)

        .def("clear", &Snapshot::SnapshotNode::clear)
        .def("push_back", py::overload_cast<const std::shared_ptr<Snapshot::SnapshotNode>&>(&Snapshot::SnapshotNode::push_back), py::return_value_policy::reference_internal);
}

}
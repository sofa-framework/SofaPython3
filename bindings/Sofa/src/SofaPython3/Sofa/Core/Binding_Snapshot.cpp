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

#include "Binding_Snapshot.h"
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <SofaPython3/Sofa/Core/Binding_Snapshot.h>
#include <SofaPython3/PythonFactory.h>

using sofa::core::objectmodel::Snapshot;

namespace py { using namespace pybind11; }

namespace sofapython3 {

    void moduleAddSnapshot(py::module &m)
    {
        py::class_<Snapshot_Python>(m, "Snapshot_Python")
        .def(py::init<>())
        .def("push_back", &sofapython3::Snapshot_Python::push_back)
        .def("getNumberOfSnapshot",&Snapshot_Python::getNumberOfSnapshot)
        .def_readwrite("m_snapshots", &Snapshot_Python::m_snapshots);
    }
}
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
#include <pybind11/stl.h>

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_BaseContext.h>
#include <SofaPython3/PythonFactory.h>
#include <sofa/core/BaseState.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/behavior/BaseMechanicalState.h>
#include <sofa/core/topology/Topology.h>
#include <sofa/core/topology/BaseMeshTopology.h>


namespace py { using namespace pybind11; }

using namespace sofa::core::objectmodel;
using namespace sofa::core::topology;

namespace sofapython3 {

void moduleAddBaseMeshTopology(py::module& m) {
    py::class_<BaseMeshTopology, Base, py_shared_ptr<BaseMeshTopology>> c (m, "BaseMeshTopology");

    /// register the ContactListener binding in the downcasting subsystem
    PythonFactory::registerType<BaseMeshTopology>([](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<BaseMeshTopology*>(object));
    });

    c.def("getNbPoints", &BaseMeshTopology::getNbPoints);
    c.def("getNbLines", &BaseMeshTopology::getNbLines);
    c.def("getNbEdges", &BaseMeshTopology::getNbEdges);
    c.def("getNbTriangles", &BaseMeshTopology::getNbTriangles);
    c.def("getNbTetrahedra", &BaseMeshTopology::getNbTetrahedra);
    c.def("getNbHexahedra", &BaseMeshTopology::getNbHexahedra);
    c.def("getNbQuads", &BaseMeshTopology::getNbQuads);
    c.def("getNbTetras", &BaseMeshTopology::getNbTetras);

    c.def("getEdge", 
      [] (BaseMeshTopology &self, const sofa::Index & index) -> std::array<sofa::Index, 2> {
          const auto & e = self.getEdge(index);
          return {{e[0], e[1]}};
      },
      py::arg("index")
    );

    c.def("getLocalEdgesInTetrahedron", 
      [] (const BaseMeshTopology & self, const sofa::Index & index) -> std::array<sofa::Index, 2> {
          const auto & e = self.getLocalEdgesInTetrahedron(index);
          return {{e[0], e[1]}};
      },
      py::arg("index"),
      "Returns for each index (between 0 and 5) the two vertex indices that are adjacent to that edge."
    );

    c.def("getEdgesInTetrahedron", 
      [] (BaseMeshTopology & self, const sofa::Index & index) -> std::array<sofa::Index, 6> {
          const auto & e = self.getEdgesInTetrahedron(index);
          return {{e[0], e[1], e[2], e[3], e[4], e[5]}};
      },
      py::arg("index"),
      "Returns the set of edges adjacent to a given tetrahedron."
    );

    c.def("getTetrahedron", 
      [] (BaseMeshTopology & self, const sofa::Index & index) -> std::array<sofa::Index, 4> {
          const auto & n = self.getTetrahedron(index);
          return {{n[0], n[1], n[2], n[3]}};
      },
      py::arg("index"),
      "Returns the vertices of Tetrahedron at index."
    );
}

} // namespace sofapython3

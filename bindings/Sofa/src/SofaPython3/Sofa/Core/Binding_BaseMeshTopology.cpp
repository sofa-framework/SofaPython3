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
#include <SofaPython3/Sofa/Core/Binding_BaseMeshTopology.h>
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


auto getPythonClassForBaseMeshTopology(py::module& m)
{
    /// Register the BaseData binding into the pybind11 system.
   static py::class_<BaseMeshTopology, Topology, py_shared_ptr<BaseMeshTopology>> basemesh(m, "BaseMeshTopology", "API providing all topology-related functions");
   return basemesh;
}

void moduleForwardAddBaseMeshTopology(py::module& m)
{
    getPythonClassForBaseMeshTopology(m);
}

void moduleAddBaseMeshTopology(py::module& m) {
    auto c = getPythonClassForBaseMeshTopology(m);

    /// register the BaseMeshTopology binding in the downcasting subsystem
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
      py::arg("index"),
      "Returns the vertices of Edge at index."
    );

    c.def("getTriangle",
      [] (BaseMeshTopology &self, const sofa::Index & index) -> std::array<sofa::Index, 3> {
          const auto & t = self.getTriangle(index);
          return {{t[0], t[1], t[2]}};
      },
      py::arg("index"),
      "Returns the vertices of Triangle at index."
    );

    c.def("getQuad",
      [] (BaseMeshTopology &self, const sofa::Index & index) -> std::array<sofa::Index, 4> {
          const auto & q = self.getQuad(index);
          return {{q[0], q[1], q[2], q[3]}};
      },
      py::arg("index"),
      "Returns the vertices of Quad at index."
    );

    c.def("getTetrahedron",
      [] (BaseMeshTopology & self, const sofa::Index & index) -> std::array<sofa::Index, 4> {
          const auto & n = self.getTetrahedron(index);
          return {{n[0], n[1], n[2], n[3]}};
      },
      py::arg("index"),
      "Returns the vertices of Tetrahedron at index."
    );

    c.def("getHexahedron",
      [] (BaseMeshTopology & self, const sofa::Index & index) -> std::array<sofa::Index, 8> {
          const auto & n = self.getHexahedron(index);
          return {{n[0], n[1], n[2], n[3], n[4], n[5], n[6], n[7]}};
      },
      py::arg("index"),
      "Returns the vertices of Hexahedron at index."
    );

    c.def("getLocalEdgesInTetrahedron", 
      [] (const BaseMeshTopology & self, const sofa::Index & index) -> std::array<sofa::Index, 2> {
          const auto & e = self.getLocalEdgesInTetrahedron(index);
          return {{e[0], e[1]}};
      },
      py::arg("index"),
      "Returns for each index (between 0 and 5) the two vertex indices that are adjacent to that edge."
    );

    c.def("getEdgesAroundVertex",
      [] (BaseMeshTopology & self, const sofa::Index & index)
      {
          return self.getEdgesAroundVertex(index);
      },
      py::arg("index"),
      "Returns the set of edges adjacent to a given vertex."
    );

    c.def("getEdgesInTriangle",
      [] (BaseMeshTopology & self, const sofa::Index & index) -> std::array<sofa::Index, 3> {
          const auto & e = self.getEdgesInTriangle(index);
          return {{e[0], e[1], e[2]}};
      },
      py::arg("index"),
      "Returns the set of edges adjacent to a given triangle."
    );

    c.def("getEdgesInQuad",
      [] (BaseMeshTopology & self, const sofa::Index & index) -> std::array<sofa::Index, 4> {
          const auto & e = self.getEdgesInQuad(index);
          return {{e[0], e[1], e[2], e[3]}};
      },
      py::arg("index"),
      "Returns the set of edges adjacent to a given quad."
    );

    c.def("getEdgesInTetrahedron", 
      [] (BaseMeshTopology & self, const sofa::Index & index) -> std::array<sofa::Index, 6> {
          const auto & e = self.getEdgesInTetrahedron(index);
          return {{e[0], e[1], e[2], e[3], e[4], e[5]}};
      },
      py::arg("index"),
      "Returns the set of edges adjacent to a given tetrahedron."
    );

    c.def("getEdgesInHexahedron",
      [] (BaseMeshTopology & self, const sofa::Index & index) -> std::array<sofa::Index, 12> {
          const auto & e = self.getEdgesInHexahedron(index);
          return {{e[0], e[1], e[2], e[3], e[4], e[5], e[6], e[7], e[8], e[9], e[10], e[11]}};
      },
      py::arg("index"),
      "Returns the set of edges adjacent to a given hexahedron."
    );

    c.def("getTrianglesAroundVertex",
      [] (BaseMeshTopology & self, const sofa::Index & index)
      {
          return self.getTrianglesAroundVertex(index);
      },
      py::arg("index"),
      "Returns the set of triangles adjacent to a given vertex."
    );

    c.def("getTrianglesAroundEdge",
      [] (BaseMeshTopology & self, const sofa::Index & index)
      {
          return self.getTrianglesAroundEdge(index);
      },
      py::arg("index"),
      "Returns the set of triangles adjacent to a given edge."
    );

    c.def("getTrianglesInTetrahedron",
      [] (BaseMeshTopology & self, const sofa::Index & index) -> std::array<sofa::Index, 4>
      {
          const auto& triangles = self.getTrianglesInTetrahedron(index);
          return {{triangles[0], triangles[1], triangles[2], triangles[3]}};
      },
      py::arg("index"),
      "Returns the set of triangles adjacent to a given tetrahedron."
    );

    c.def("getQuadsAroundVertex",
      [] (BaseMeshTopology & self, const sofa::Index & index)
      {
          return self.getQuadsAroundVertex(index);
      },
      py::arg("index"),
      "Returns the set of quads adjacent to a given vertex."
    );

    c.def("getQuadsAroundEdge",
      [] (BaseMeshTopology & self, const sofa::Index & index)
      {
          return self.getQuadsAroundEdge(index);
      },
      py::arg("index"),
      "Returns the set of quads adjacent to a given edge."
    );

    c.def("getQuadsInHexahedron",
      [] (BaseMeshTopology & self, const sofa::Index & index) -> std::array<sofa::Index, 6>
      {
          const auto& q =  self.getQuadsInHexahedron(index);
          return {{q[0], q[1], q[2], q[3], q[4], q[5]}};
      },
      py::arg("index"),
      "Returns the set of quads adjacent to a given hexahedron."
    );

    c.def("getTetrahedraAroundVertex",
      [] (BaseMeshTopology & self, const sofa::Index & index)
      {
          return self.getTetrahedraAroundVertex(index);
      },
      py::arg("index"),
      "Returns the set of tetrahedra adjacent to a given vertex."
    );

    c.def("getTetrahedraAroundEdge",
      [] (BaseMeshTopology & self, const sofa::Index & index)
      {
          return self.getTetrahedraAroundEdge(index);
      },
      py::arg("index"),
      "Returns the set of tetrahedra adjacent to a given edge."
    );

    c.def("getTetrahedraAroundTriangle",
      [] (BaseMeshTopology & self, const sofa::Index & index)
      {
          return self.getTetrahedraAroundTriangle(index);
      },
      py::arg("index"),
      "Returns the set of tetrahedra adjacent to a given triangle."
    );

    c.def("getHexahedraAroundVertex",
      [] (BaseMeshTopology & self, const sofa::Index & index)
      {
          return self.getHexahedraAroundVertex(index);
      },
      py::arg("index"),
      "Returns the set of hexahedra adjacent to a given vertex."
    );

    c.def("getHexahedraAroundEdge",
      [] (BaseMeshTopology & self, const sofa::Index & index)
      {
          return self.getHexahedraAroundEdge(index);
      },
      py::arg("index"),
      "Returns the set of hexahedra adjacent to a given edge."
    );

    c.def("getHexahedraAroundQuad",
      [] (BaseMeshTopology & self, const sofa::Index & index)
      {
          return self.getHexahedraAroundQuad(index);
      },
      py::arg("index"),
      "Returns the set of hexahedra adjacent to a given quad."
    );

    c.def("getVerticesAroundVertex",
      [] (BaseMeshTopology & self, const sofa::Index & index)
      {
          return self.getVerticesAroundVertex(index);
      },
      py::arg("index"),
      "Returns the set of vertices adjacent to a given vertex (i.e. sharing an edge)"
    );

}

} // namespace sofapython3

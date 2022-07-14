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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Binding_SparseGridTopology.h"
#include "Binding_SparseGridTopology_doc.h"

#include <sofa/component/topology/container/grid/SparseGridTopology.h>
#include <SofaPython3/PythonFactory.h>

using sofa::component::topology::container::grid::SparseGridTopology;
using sofa::core::objectmodel::BaseObject;

PYBIND11_DECLARE_HOLDER_TYPE(Base, sofa::core::sptr<Base>, true)

namespace sofapython3 {

namespace py { using namespace pybind11; }

void moduleAddSparseGridTopology(pybind11::module& m) {
    py::class_<SparseGridTopology, sofa::core::objectmodel::BaseObject,sofa::core::sptr<SparseGridTopology>>
    c(m, "SparseGridTopology", doc::SofaBaseTopology::sparseGridTopologyClass);

    // getRegularGrid ()
    c.def("getRegularGrid", [](SparseGridTopology & self){
        return self._regularGrid.get();
    }, doc::SofaBaseTopology::getRegularGrid);

    // findCube (pos)
    c.def("findCube", [](SparseGridTopology & self, const py::list & l) {
        sofa::type::Vector3 pos;
        pos[0] = l[0].cast<double>();
        pos[1] = l[1].cast<double>();
        pos[2] = l[2].cast<double>();

        SReal fx=0., fy=0., fz=0.;
        SReal dx=self._regularGrid->getDx()[0];
        SReal dy=self._regularGrid->getDy()[1];
        SReal dz=self._regularGrid->getDz()[2];

        int cube_id = self.findCube(pos, fx, fy, fz);

        if (cube_id < 0) {
            // It may be a node
            const int node_id = self._regularGrid->findPoint(pos);
            if (node_id > -1) {
                // It is a node, find the first hexa connected to this node that is either inside or on the boundary
                const auto&  hexas = self._regularGrid->getHexahedraAroundVertex(node_id);
                for (const auto & hexa_id : hexas) {
                    if (self._indicesOfRegularCubeInSparseGrid[hexa_id] > -1) {
                        auto p = pos-self._regularGrid->d_p0.getValue();

                        SReal x = p[0]/dx;
                        SReal y = p[1]/dy;
                        SReal z = p[2]/dz;

                        int ix = int(x+1000000)-1000000; // Do not round toward 0...
                        int iy = int(y+1000000)-1000000;
                        int iz = int(z+1000000)-1000000;

                        fx = x-ix;
                        fy = y-iy;
                        fz = z-iz;
                        cube_id = self._indicesOfRegularCubeInSparseGrid[hexa_id];
                        break;
                    }
                }
            }
        }

        return py::make_tuple(cube_id, fx, fy, fz);
    }, doc::SofaBaseTopology::findCube);

    // getRegularGridNodeIndex (sparse_grid_node_index)
    c.def("getRegularGridNodeIndex", [](SparseGridTopology & self, size_t sparse_grid_node_index) {
        if (sparse_grid_node_index >= self.getNbPoints()) {
            throw py::index_error(
                "Node index " + std::to_string(sparse_grid_node_index) + " is greater then the number of nodes contained by "
                "the sparse grid (" + std::to_string(self.getNbPoints()) + " nodes).");
        }

        const auto pos = self.getPointPos(sparse_grid_node_index);
        return self._regularGrid->findPoint(pos);
    }, doc::SofaBaseTopology::getRegularGridNodeIndex);

    // getRegularGridCubeIndex (sparse_grid_cube_index)
    c.def("getRegularGridCubeIndex", [](SparseGridTopology & self, size_t sparse_grid_cube_index) {
        if (sparse_grid_cube_index >= self.getNbHexahedra()) {
            throw py::index_error(
                    "Cube index " + std::to_string(sparse_grid_cube_index) + " is greater then the number of cubes contained by "
                         "the sparse grid (" + std::to_string(self.getNbHexahedra()) + " cubes).");
        }

        return self._indicesOfCubeinRegularGrid[sparse_grid_cube_index];
    }, doc::SofaBaseTopology::getRegularGridCubeIndex);

    // getBoundaryCells ()
    c.def("getBoundaryCells", [] (SparseGridTopology & self) {
        std::list<unsigned int> indices;
        for (std::size_t hexa_id = 0; hexa_id < self.getNbHexahedra(); ++hexa_id) {
            if (self.getType((int)hexa_id) == SparseGridTopology::BOUNDARY)
                indices.push_back(hexa_id);
        }
        return indices;
    }, doc::SofaBaseTopology::getBoundaryCells);

    // getBoundaryNodes ()
    c.def("getBoundaryNodes", [] (SparseGridTopology & self) {
        std::list<unsigned int> indices;
        for (std::size_t node_id = 0; node_id < self.getNbPoints(); ++node_id) {
            auto nb_of_connected_hexa = self.getHexahedraAroundVertex(node_id).size();
            if (nb_of_connected_hexa < 7)
                indices.push_back(node_id);
        }
        return indices;
    }, doc::SofaBaseTopology::getBoundaryNodes);

    // getNodeIndicesOfCube (cube_index)
    c.def("getNodeIndicesOfCube", [](SparseGridTopology & self, size_t cube_index) {
        if (cube_index >= self.getNbHexahedra()) {
            throw py::index_error(
                    "Cube index " + std::to_string(cube_index) + " is greater then the number of cubes contained by "
                         "the sparse grid (" + std::to_string(self.getNbHexahedra()) + " cubes).");
        }
        const auto hexa = self.getHexahedron(cube_index);
        std::array<sofa::core::topology::Topology::PointID, 8> indices = {};
        for (std::size_t i = 0; i < 8; ++i) {
            indices[i] = hexa[i];
        }
        return indices;
    }, doc::SofaBaseTopology::getRegularGridCubeIndex);

    /// register the binding in the downcasting subsystem
    PythonFactory::registerType<SparseGridTopology>([](sofa::core::objectmodel::Base* object){
        return py::cast(dynamic_cast<SparseGridTopology*>(object));
    });
}

} // namespace sofapython3
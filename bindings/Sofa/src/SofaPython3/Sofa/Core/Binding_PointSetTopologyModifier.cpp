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
#include <SofaPython3/PythonFactory.h>
#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_BaseContext.h>
#include <SofaPython3/Sofa/Core/Binding_PointSetTopologyModifier.h>
#include <pybind11/stl.h>
#include <sofa/component/topology/container/dynamic/PointSetTopologyModifier.h>

typedef sofa::component::topology::container::dynamic::PointSetTopologyModifier PointSetTopologyModifier;
typedef sofa::core::topology::BaseMeshTopology::PointID PointID;

namespace py {
using namespace pybind11;
}

using namespace sofa::core::objectmodel;
using namespace sofa::core::topology;

namespace sofapython3 {

void moduleAddPointSetTopologyModifier(py::module& m) {
  py::class_<PointSetTopologyModifier, Base, py_shared_ptr<PointSetTopologyModifier>> c(m, "PointSetTopologyModifier");

  /// register the PointSetTopologyModifier binding in the downcasting subsystem
  PythonFactory::registerType<PointSetTopologyModifier>([](sofa::core::objectmodel::Base* object) {
    return py::cast(dynamic_cast<PointSetTopologyModifier*>(object));
  });

  c.def("addPoints",
        [](PointSetTopologyModifier& self, const sofa::Size nPoints, const bool addDOF) {
          self.addPoints(nPoints, addDOF);
        });
  c.def("removePoints",
        [](PointSetTopologyModifier& self, const std::vector<unsigned int>& indices, const bool removeDOF) {
          sofa::type::vector<PointID> index_vector;
          index_vector.reserve(indices.size());
          for (const auto i : indices) {
            index_vector.emplace_back(i);
          }

          self.removePoints(index_vector, removeDOF);
        });
}

}  // namespace sofapython3

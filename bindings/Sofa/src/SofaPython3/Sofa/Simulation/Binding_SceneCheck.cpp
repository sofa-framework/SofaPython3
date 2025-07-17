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
#include <SofaPython3/Sofa/Simulation/Binding_SceneCheck.h>
#include <SofaPython3/Sofa/Simulation/Binding_SceneCheck_doc.h>
#include <sofa/simulation/SceneCheck.h>

namespace sofapython3
{

namespace py { using namespace pybind11; }

void moduleAddSceneCheck(pybind11::module &m)
{
    // create a python binding for the C++ class LinearSpring from SofaDeformable
    py::class_<sofa::simulation::SceneCheck, std::shared_ptr<sofa::simulation::SceneCheck>> s
    (m, "SceneCheck", sofapython3::doc::simulation::SceneCheckClass);

    s.def("getName", &sofa::simulation::SceneCheck::getName, sofapython3::doc::simulation::SceneCheck_getName);
    s.def("getDesc", &sofa::simulation::SceneCheck::getDesc, sofapython3::doc::simulation::SceneCheck_getDesc);
}

}

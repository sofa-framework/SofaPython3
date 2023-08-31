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


#include <sofa/helper/logging/Messaging.h>
using sofa::helper::logging::Message;

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_BaseContext.h>
#include <SofaPython3/Sofa/Core/Binding_BaseObject.h>
#include <SofaPython3/Sofa/Core/Binding_DataDict.h>
#include <SofaPython3/Sofa/Core/Binding_BaseData.h>
#include <SofaPython3/Sofa/Core/Binding_BaseCamera.h>
#include <SofaPython3/Sofa/Core/Binding_ForceField.h>
#include <SofaPython3/Sofa/Core/Binding_Mass.h>
#include <SofaPython3/Sofa/Core/Binding_ContactListener.h>
#include <SofaPython3/Sofa/Core/Binding_Context.h>
#include <SofaPython3/Sofa/Core/Binding_Controller.h>
#include <SofaPython3/Sofa/Core/Binding_DataEngine.h>
#include <SofaPython3/Sofa/Core/Binding_ObjectFactory.h>
#include <SofaPython3/Sofa/Core/Binding_LinkPath.h>
#include <SofaPython3/Sofa/Core/Binding_Node.h>
#include <SofaPython3/Sofa/Core/Binding_NodeIterator.h>
#include <SofaPython3/Sofa/Core/Binding_Prefab.h>
#include <SofaPython3/Sofa/Core/Binding_BaseLink.h>
#include <SofaPython3/Sofa/Core/Binding_PointSetTopologyModifier.h>
#include <SofaPython3/Sofa/Core/Binding_PythonScriptEvent.h>
#include <SofaPython3/Sofa/Core/Binding_Topology.h>
#include <SofaPython3/Sofa/Core/Binding_BaseMeshTopology.h>

#include <SofaPython3/Sofa/Core/Data/Binding_DataString.h>
#include <SofaPython3/Sofa/Core/Data/Binding_DataLink.h>
#include <SofaPython3/Sofa/Core/Data/Binding_DataVectorString.h>
#include <SofaPython3/Sofa/Core/Data/Binding_DataContainer.h>

namespace sofapython3
{

/// The first parameter must be named the same as the module file to load.
PYBIND11_MODULE(Core, core)
{
    core.doc() = R"doc(
        Contains the general object hierarchy and data structures

        A SOFA simulation is referred as a "scene". A scene contains several object. Each object is implemented as the
        aggregation of components. Groups of component are grouped and organized logically using a specific container called
        Node. Nodes are hierarchically organized in a scene graph.
        For more, see the `SOFA lexicography <https://www.sofa-framework.org/community/doc/using-sofa/lexicography/>`_.

        Commonly used classes:
            .. autosummary::
                :nosignatures:

                Node
                Object
                Controller
                ForceFieldVec3d
                Data

        Example:
            .. code-block:: python

                import Sofa.Core

                # Create a new node
                n = Sofa.Core.Node("MyNode")

                # Create a new node
                n = Sofa.Core.Node("MyNode"")
                # Add a child node name 'Node2'
                n.addChild("Node2")

                # Add a mechanical component to MyNode
                n.addObject("MechanicalObject", name="dofs")

                Sofa.Simulation.init(root)
                Sofa.Simulation.print(root)
       )doc";

    moduleAddPythonScriptEvent();
    moduleAddDataDict(core);
    moduleAddDataDictIterator(core);
    moduleAddBase(core);
    moduleAddBaseContext(core);
    moduleAddBaseData(core);
    moduleAddDataContainerContext(core);
    moduleAddDataContainer(core);
    moduleAddDataString(core);
    moduleAddDataLink(core);
    moduleAddDataVectorString(core);
    moduleAddBaseObject(core);
    moduleAddBaseCamera(core);
    moduleAddContactListener(core);
    moduleAddContext(core);
    moduleAddController(core);
    moduleAddDataEngine(core);
    moduleAddForceField(core);
    moduleAddMass(core);
    moduleAddObjectFactory(core);
    moduleAddLinkPath(core);
    moduleAddNode(core);
    moduleAddNodeIterator(core);
    moduleAddPrefab(core);
    moduleAddBaseLink(core);
    moduleAddTopology(core);
    moduleAddBaseMeshTopology(core);
    moduleAddPointSetTopologyModifier(core);
}

} ///namespace sofapython3

/******************************************************************************
*                              SofaPython3 plugin                             *
*                  (c) 2019 CNRS, University of Lille, INRIA                  *
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
* Authors: see Authors.txt                                                    *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
/********************************************************************
 Contributors:
    - damien.marchal@univ-lille.fr
    - bruno.josue.marques@inria.fr
    - eve.le-guillou@centrale.centralelille.fr
    - jean-nicolas.brunet@inria.fr
    - thierry.gaugry@inria.fr
********************************************************************/


#include <sofa/helper/logging/Messaging.h>
using sofa::helper::logging::Message;

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_BaseContext.h>
#include <SofaPython3/Sofa/Core/Binding_BaseObject.h>
#include <SofaPython3/Sofa/Core/Binding_DataDict.h>
#include <SofaPython3/Sofa/Core/Binding_BaseData.h>
#include <SofaPython3/Sofa/Core/Binding_BaseCamera.h>
#include <SofaPython3/Sofa/Core/Binding_ForceField.h>
#include <SofaPython3/Sofa/Core/Binding_ContactListener.h>
#include <SofaPython3/Sofa/Core/Binding_Context.h>
#include <SofaPython3/Sofa/Core/Binding_Controller.h>
#include <SofaPython3/Sofa/Core/Binding_DataEngine.h>
#include <SofaPython3/Sofa/Core/Binding_ObjectFactory.h>
#include <SofaPython3/Sofa/Core/Binding_Node.h>
#include <SofaPython3/Sofa/Core/Binding_NodeIterator.h>
#include <SofaPython3/Sofa/Core/Binding_Prefab.h>
#include <SofaPython3/Sofa/Core/Binding_BaseLink.h>
#include <SofaPython3/Sofa/Core/Binding_PythonScriptEvent.h>

#include <SofaPython3/Sofa/Core/Data/Binding_DataString.h>
#include <SofaPython3/Sofa/Core/Data/Binding_DataLink.h>
#include <SofaPython3/Sofa/Core/Data/Binding_DataVectorString.h>
#include <SofaPython3/Sofa/Core/Data/Binding_DataContainer.h>

#include <sofa/core/init.h>
#include <sofa/helper/init.h>
#include <sofa/simulation/init.h>
#include <sofa/defaulttype/init.h>
#include <SofaBaseCollision/initSofaBaseCollision.h>
#include <SofaBaseVisual/initSofaBaseVisual.h>
#include <SofaBaseUtils/initSofaBaseUtils.h>

namespace sofapython3
{

/// The first parameter must be named the same as the module file to load.
PYBIND11_MODULE(Core, core)
{
    // These are needed to force the dynamic loading of module dependencies (found in CMakeLists.txt)
    sofa::core::init();
    sofa::helper::init();
    sofa::simulation::core::init();
    sofa::defaulttype::init();
    sofa::component::initSofaBaseCollision();
    sofa::component::initSofaBaseVisual();
    sofa::component::initSofaBaseUtils(); // Needed to add "RequiredPlugin" components

    core.doc() = R"doc(
           Scene components
           -----------------------

           Core components
           ^^^^^^^^^^^^^^^

           .. autosummary::
               :toctree: _autosummary/_autosummary

               Sofa.Core.Data
               Sofa.Core.Link
               Sofa.Core.Node
               Sofa.Core.RawPrefab
               Sofa.Core.Object
               Sofa.Core.Camera

           Overridable components
           ^^^^^^^^^^^^^^^^^^^^^^

           .. autosummary::
               :toctree: _autosummary/_autosummary

               Sofa.Core.Controller
               Sofa.Core.ForceField
               Sofa.Core.DataEngine


           Utilitary classes
           ^^^^^^^^^^^^^^^^^

           .. autosummary::
               :toctree: _autosummary/_autosummary

               Sofa.Core.DataDict
               Sofa.Core.DataDictIterator
               Sofa.Core.DataContainer
               Sofa.Core.DataString
               Sofa.Core.DataVectorString
               Sofa.Core.NodeIterator
               #Sofa.Core.WriteAccessor
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
    moduleAddObjectFactory(core);
    moduleAddNode(core);
    moduleAddNodeIterator(core);
    moduleAddPrefab(core);
    moduleAddBaseLink(core);
}

} ///namespace sofapython3

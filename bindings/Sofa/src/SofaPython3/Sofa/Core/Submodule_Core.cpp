
#include <sofa/core/objectmodel/BaseNode.h>
#include <sofa/core/objectmodel/BaseContext.h>
#include <sofa/core/behavior/BaseForceField.h>
#include <sofa/core/objectmodel/Context.h>

#include "Binding_Base.h"
#include "Binding_BaseObject.h"
#include "Binding_DataDict.h"
#include "Binding_BaseData.h"
#include "Binding_BaseCamera.h"
#include "Binding_ForceField.h"
#include "Binding_Controller.h"
#include "Binding_DataEngine.h"
#include "Binding_Node.h"
#include "Binding_Simulation.h"
#include "Binding_BaseLink.h"
#include "Binding_PythonScriptEvent.h"

#include "Data/Binding_DataString.h"
#include "Data/Binding_DataVectorString.h"
#include "Data/Binding_DataContainer.h"

#include "Submodule_Core.h"



namespace sofapython3
{

/// The first parameter must be named the same as the module file to load.
PYBIND11_MODULE(Core, core)
{
    core.doc() = R"doc(
           Scene components
           -----------------------

           Core components:

           .. autosummary::
               :toctree: _autosummary/_autosummary

               Sofa.Core.Data
               Sofa.Core.Link
               Sofa.Core.Node
               Sofa.Core.Object
               Sofa.Core.Camera

           Overridable components:

           .. autosummary::
               :toctree: _autosummary/_autosummary

               Sofa.Core.Controller
               Sofa.Core.ForceField
               Sofa.Core.DataEngine


           Utilitary classes:

           .. autosummary::
               :toctree: _autosummary/_autosummary

               Sofa.Core.DataDict
               Sofa.Core.DataDictIterator
               Sofa.Core.DataContainer
               Sofa.Core.DataString
               Sofa.Core.DataVectorString
               #Sofa.Core.WriteAccessor
       )doc";

    moduleAddPythonScriptEvent();
    moduleAddDataDict(core);
    moduleAddDataDictIterator(core);
    moduleAddBase(core);
    moduleAddBaseData(core);
    moduleAddDataContainerContext(core);
    moduleAddDataContainer(core);
    moduleAddDataString(core);
    moduleAddDataVectorString(core);
    moduleAddBaseObject(core);
    moduleAddBaseCamera(core);
    moduleAddController(core);
    moduleAddDataEngine(core);
    moduleAddForceField(core);

    py::class_<sofa::core::objectmodel::BaseNode,
            sofa::core::objectmodel::Base,
            sofa::core::objectmodel::BaseNode::SPtr>(core, "BaseNode");

    py::class_<sofa::core::objectmodel::BaseContext,
            sofa::core::objectmodel::Base,
            sofa::core::objectmodel::BaseContext::SPtr>(core, "BaseContext");

    py::class_<sofa::core::objectmodel::Context,
            sofa::core::objectmodel::BaseContext,
            sofa::core::objectmodel::Context::SPtr>(core, "Context");

    moduleAddNode(core);
    moduleAddBaseLink(core);

}

} ///namespace sofapython3

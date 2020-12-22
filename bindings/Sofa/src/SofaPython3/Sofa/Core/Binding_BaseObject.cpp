/*********************************************************************
Copyright 2019, CNRS, University of Lille, INRIA

This file is part of sofaPython3

sofaPython3 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

sofaPython3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with sofaqtquick. If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/
/********************************************************************
 Contributors:
    - damien.marchal@univ-lille.fr
    - bruno.josue.marques@inria.fr
    - eve.le-guillou@centrale.centralelille.fr
    - jean-nicolas.brunet@inria.fr
    - thierry.gaugry@inria.fr
********************************************************************/

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_BaseObject.h>
#include <SofaPython3/Sofa/Core/Binding_BaseObject_doc.h>
#include <SofaPython3/Sofa/Core/Binding_Controller.h>
#include <SofaPython3/PythonFactory.h>

#include <sofa/core/ObjectFactory.h>

// Imports for getCategories
#include <sofa/core/objectmodel/ContextObject.h>
#include <sofa/core/visual/VisualModel.h>
#include <sofa/core/BehaviorModel.h>
#include <sofa/core/CollisionModel.h>
#include <sofa/core/behavior/BaseMechanicalState.h>
#include <sofa/core/behavior/BaseForceField.h>
#include <sofa/core/behavior/BaseInteractionForceField.h>
#include <sofa/core/behavior/BaseProjectiveConstraintSet.h>
#include <sofa/core/behavior/BaseConstraintSet.h>
#include <sofa/core/DataEngine.h>
#include <sofa/core/topology/TopologicalMapping.h>
#include <sofa/core/behavior/BaseMass.h>
#include <sofa/core/behavior/OdeSolver.h>
#include <sofa/core/behavior/ConstraintSolver.h>
#include <sofa/core/behavior/BaseConstraintCorrection.h>
#include <sofa/core/behavior/LinearSolver.h>
#include <sofa/core/behavior/BaseAnimationLoop.h>
#include <sofa/core/topology/BaseTopology.h>
#include <sofa/core/loader/BaseLoader.h>
#include <sofa/core/collision/CollisionAlgorithm.h>
#include <sofa/core/collision/Pipeline.h>
#include <sofa/core/collision/Intersection.h>
#include <sofa/core/objectmodel/ConfigurationSetting.h>

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

using sofa::core::objectmodel::BaseData;
using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::BaseObject;

namespace sofapython3
{
py::object getItem(const BaseObject& self, const std::string& path)
{
    if (path.empty())
        return py::cast(self);

    BaseData* data = self.findData(path);
    if (data)
        return py::cast(data);
    sofa::core::objectmodel::BaseLink* link = self.findLink(path);
    if (link)
        return py::cast(link);
    throw py::value_error("Invalid syntax"); // should never get there
}

std::string getLinkPath(const BaseObject *self)
{
    return std::string("@")+self->getPathName();
}

void computeBBox(BaseObject *self)
{
    self->computeBBox(sofa::core::ExecParams::defaultInstance(), false);
}

py::list getSlaves(BaseObject &self)
{
   const BaseObject::VecSlaves& slaves = self.getSlaves();
   py::list slaveList;
   for (auto slave : slaves){
       slaveList.append(py::cast(slave));
   }
   return slaveList;
}

py::object getContext(const BaseObject &self)
{
    const sofa::core::objectmodel::BaseContext* context =  self.getContext();
    if (context){
        return PythonFactory::toPython(const_cast<sofa::core::objectmodel::BaseContext*>(context));
    }
    return py::none();
}

py::object getMaster(const BaseObject &self)
{
    const BaseObject* master = self.getMaster();
    if (master){
        return py::cast(master);
    }
    return py::none();
}

py::object getTarget(BaseObject *self)
{
    if (!self)
        return py::none();
    sofa::core::ObjectFactory::ClassEntry entry = sofa::core::ObjectFactory::getInstance()->getEntry(self->getClassName());
    if (!entry.creatorMap.empty())
    {
        sofa::core::ObjectFactory::CreatorMap::iterator it = entry.creatorMap.find(self->getTemplateName());
        if (it != entry.creatorMap.end() && *it->second->getTarget())
        {
            return py::cast(it->second->getTarget()) ;
        }
    }
    return py::none() ;
}

py::object getCategories(BaseObject *self)
{
    const sofa::core::objectmodel::BaseClass* mclass=self->getClass();
    std::vector<std::string> categories;
    if (mclass->hasParent(sofa::core::objectmodel::ContextObject::GetClass()))
        categories.push_back("ContextObject");
    if (mclass->hasParent(sofa::core::visual::VisualModel::GetClass()))
        categories.push_back("VisualModel");
    if (mclass->hasParent(sofa::core::BehaviorModel::GetClass()))
        categories.push_back("BehaviorModel");
    if (mclass->hasParent(sofa::core::CollisionModel::GetClass()))
        categories.push_back("CollisionModel");
    if (mclass->hasParent(sofa::core::behavior::BaseMechanicalState::GetClass()))
        categories.push_back("MechanicalState");
    // A Mass is a technically a ForceField, but we don't want it to appear in the ForceField category
    if (mclass->hasParent(sofa::core::behavior::BaseForceField::GetClass()) && !mclass->hasParent(sofa::core::behavior::BaseMass::GetClass()))
        categories.push_back("ForceField");
    if (mclass->hasParent(sofa::core::behavior::BaseInteractionForceField::GetClass()))
        categories.push_back("InteractionForceField");
    if (mclass->hasParent(sofa::core::behavior::BaseProjectiveConstraintSet::GetClass()))
        categories.push_back("ProjectiveConstraintSet");
    if (mclass->hasParent(sofa::core::behavior::BaseConstraintSet::GetClass()))
        categories.push_back("ConstraintSet");
    if (mclass->hasParent(sofa::core::BaseMapping::GetClass()))
        categories.push_back("Mapping");
    if (mclass->hasParent(sofa::core::DataEngine::GetClass()))
        categories.push_back("Engine");
    if (mclass->hasParent(sofa::core::topology::TopologicalMapping::GetClass()))
        categories.push_back("TopologicalMapping");
    if (mclass->hasParent(sofa::core::behavior::BaseMass::GetClass()))
        categories.push_back("Mass");
    if (mclass->hasParent(sofa::core::behavior::OdeSolver::GetClass()))
        categories.push_back("OdeSolver");
    if (mclass->hasParent(sofa::core::behavior::ConstraintSolver::GetClass()))
        categories.push_back("ConstraintSolver");
    if (mclass->hasParent(sofa::core::behavior::BaseConstraintCorrection::GetClass()))
        categories.push_back("ConstraintSolver");
    if (mclass->hasParent(sofa::core::behavior::LinearSolver::GetClass()))
        categories.push_back("LinearSolver");
    if (mclass->hasParent(sofa::core::behavior::BaseAnimationLoop::GetClass()))
        categories.push_back("AnimationLoop");
    // Just like Mass and ForceField, we don't want TopologyObject to appear in the Topology category
    if (mclass->hasParent(sofa::core::topology::Topology::GetClass()) && !mclass->hasParent(sofa::core::topology::BaseTopologyObject::GetClass()))
        categories.push_back("Topology");
    if (mclass->hasParent(sofa::core::topology::BaseTopologyObject::GetClass()))
        categories.push_back("TopologyObject");
    if (mclass->hasParent(sofa::core::behavior::BaseController::GetClass()))
        categories.push_back("Controller");
    if (mclass->hasParent(sofa::core::loader::BaseLoader::GetClass()))
        categories.push_back("Loader");
    if (mclass->hasParent(sofa::core::collision::CollisionAlgorithm::GetClass()))
        categories.push_back("CollisionAlgorithm");
    if (mclass->hasParent(sofa::core::collision::Pipeline::GetClass()))
        categories.push_back("CollisionAlgorithm");
    if (mclass->hasParent(sofa::core::collision::Intersection::GetClass()))
        categories.push_back("CollisionAlgorithm");
    if (mclass->hasParent(sofa::core::objectmodel::ConfigurationSetting::GetClass()))
        categories.push_back("ConfigurationSetting");
    if (categories.empty())
        categories.push_back("Miscellaneous");

    py::list list;
    for (unsigned int i=0; i<categories.size(); ++i)
        list.append(py::cast(categories[i].c_str())) ;
    return list;
}

std::string getAsACreateObjectParameter(BaseObject *self)
{
    return getLinkPath(self);
}

void setSrc(BaseObject &self, char *valueString, BaseObject *loader)
{
    self.setSrc(valueString,loader);
}

/// gets an item using its path (path is dot-separated, relative to the object
/// it's called upon & ONLY DESCENDING (no ../):
///
/// This method lifts most ambiguities when accessing a node, object or data
/// from a path relative to self.
///
/// examples:
/// ------------
///
/// root["node1.node2.object1.value"]
///
/// In the example above, node1 and node2 can be inferred as being nodes without performing any checks.
/// object1 can be a node or an object, but cannot be a datafield nor a link
/// value can be a node or an object (if object1 is a node), or must be a data (if object1 is an object)
py::object __getitem__(BaseObject &self, std::string s)
{
    if (s[0] == '.')
        s.erase(s.begin());
    std::list<std::string> stringlist;
    std::istringstream iss(s);
    std::string token;
    while (std::getline(iss, token, '.'))
    {
        if (!token.empty())
            stringlist.push_back(token);
    }

    // perform here the syntax checks over the string to parse

    if (stringlist.empty())
        return py::cast(self);
    if (stringlist.size() > 1)
        throw py::value_error("Invalid syntax");

    return getItem(self, s);
}

void moduleAddBaseObject(py::module& m)
{
    /// Register the BaseObject binding into the pybind11 typing system
    py::class_<BaseObject, Base, py_shared_ptr<BaseObject>>p(m, "Object", sofapython3::doc::baseObject::Class);

    /// Register the BaseObject binding into the downcasting subsystem
    PythonFactory::registerType<sofa::core::objectmodel::BaseObject>(
                [](sofa::core::objectmodel::Base* object)
    {
        return py::cast(py_shared_ptr<sofa::core::objectmodel::BaseObject>(object->toBaseObject()));
    });

    p.def("init", &BaseObject::init, sofapython3::doc::baseObject::init);
    p.def("reinit", &BaseObject::reinit, sofapython3::doc::baseObject::reinit);
    p.def("getPathName", &BaseObject::getPathName, sofapython3::doc::baseObject::getPathName);
    p.def("getLinkPath", [](const BaseObject &self){ return std::string("@") + self.getPathName(); }, sofapython3::doc::baseObject::getLink);
    p.def("getSlaves", getSlaves, sofapython3::doc::baseObject::getSlaves);
    p.def("getContext", getContext, sofapython3::doc::baseObject::getContext);
    p.def("getMaster", getMaster, sofapython3::doc::baseObject::getMaster);
    p.def("addSlave", &BaseObject::addSlave, sofapython3::doc::baseObject::addSlave);
    p.def("storeResetState", &BaseObject::storeResetState, sofapython3::doc::baseObject::storeResetState);
    p.def("reset", &BaseObject::reset, sofapython3::doc::baseObject::reset);
    p.def("getTarget", getTarget, sofapython3::doc::baseObject::getTarget);
    p.def("getCategories", getCategories, sofapython3::doc::baseObject::getCategories);
    p.def("bwdInit", &BaseObject::bwdInit, sofapython3::doc::baseObject::bwdInit);
    p.def("cleanup", &BaseObject::cleanup, sofapython3::doc::baseObject::cleanup);
    p.def("computeBBox", &computeBBox, sofapython3::doc::baseObject::computeBBox);
    p.def("getLinkPath", &getLinkPath, sofapython3::doc::baseObject::getLinkPath);
    p.def("getAsACreateObjectParameter", getAsACreateObjectParameter, sofapython3::doc::baseObject::getAsACreateObjectParameter);
    p.def("setSrc", setSrc, sofapython3::doc::baseObject::setSrc);
    p.def("computeBBox", &BaseObject::computeBBox, sofapython3::doc::baseObject::computeBBox);
    p.def("__getitem__", __getitem__, sofapython3::doc::baseObject::__getitem__);
}

}  /// namespace sofapython3

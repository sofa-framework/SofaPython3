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


/// Neede to have automatic conversion from pybind types to stl container.
#include <pybind11/stl.h>

#include <sofa/core/objectmodel/BaseData.h>
#include <sofa/simulation/Simulation.h>
using sofa::core::objectmodel::BaseData;

#include <SofaSimulationGraph/SimpleApi.h>
namespace simpleapi = sofa::simpleapi;

#include <sofa/helper/logging/Messaging.h>
using sofa::helper::logging::Message;

#include <SofaSimulationGraph/DAGNode.h>
using sofa::core::ExecParams;

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_BaseObject.h>
#include <SofaPython3/DataHelper.h>

#include <sofa/core/ObjectFactory.h>
using sofa::core::ObjectFactory;

#include <SofaPython3/PythonFactory.h>
using sofapython3::PythonFactory;

#include <SofaPython3/Sofa/Core/Binding_Node.h>
#include <SofaPython3/Sofa/Core/Binding_Node_doc.h>
#include <SofaPython3/Sofa/Core/Binding_NodeIterator.h>
#include <SofaPython3/Sofa/Core/Binding_PythonScriptEvent.h>

using sofa::core::objectmodel::BaseObjectDescription;

#include <queue>
#include <sofa/core/objectmodel/Link.h>

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }


using sofa::simulation::Node;

namespace sofapython3 {


bool checkParamUsage(BaseObjectDescription& desc)
{
    bool hasFailure = false;
    std::stringstream tmp;
    tmp <<"Unknown Attribute(s): " << msgendl;
    for( auto it : desc.getAttributeMap() )
    {
        if (!it.second.isAccessed())
        {
            hasFailure = true;
            tmp << " - \""<<it.first <<"\" with value: \"" <<std::string(it.second) << msgendl;
        }
    }
    if(!desc.getErrors().empty())
    {
        hasFailure = true;
        tmp << desc.getErrors()[0];
    }
    if(hasFailure)
    {
        throw py::type_error(tmp.str());
    }
    return hasFailure;
}

py::object getItem(Node& self, std::list<std::string>& path)
{
    if (path.empty())
        return py::cast(self);

    if (path.size() > 2)
    {
        Node* child = self.getChild(path.front());
        path.pop_front();
        return getItem(*child, path);
    }
    if (path.empty())
        return py::cast(self);
    Node* child = self.getChild(path.front());
    BaseObject* obj = self.getObject(path.front());
    BaseData* data = self.findData(path.front());
    if (child)
    {
        path.pop_front();
        if (path.empty())
            return py::cast(child);
        return getItem(*child, path);
    }
    if (obj)
    {
        path.pop_front();
        if (path.empty())
            return py::cast(obj);
        return sofapython3::getItem(*obj, path.back());
    }
    if (data)
        return py::cast(data);
    return py::cast(self); // should never get there
}

std::string getLinkPath(Node* node){
    return ("@"+node->getPathName()).c_str();
}



py_shared_ptr<Node> __init__noname() {
    auto dag_node = sofa::core::objectmodel::New<sofa::simulation::graph::DAGNode>("unnamed");
    return dag_node;
}

py_shared_ptr<Node> __init__(const std::string& name) {
    auto dag_node = sofa::core::objectmodel::New<sofa::simulation::graph::DAGNode>(name);
    return dag_node;
}

/// Method: init (beware this is not the python __init__, this is sofa's init())
void init(Node& self) { self.init(ExecParams::defaultInstance()); }

py::object addObject(Node& self, const py::object & object)
{
    try {
        auto base_object = py::cast<py_shared_ptr<BaseObject>>(object);
        if (self.addObject(base_object))
            return object;
    } catch (...) {
        throw py::type_error("Trying to add an object that isn't derived from sofa::core::objectmodel::BaseObject.");
    }
    return py::none();
}

void removeObject(Node& self, BaseObject* object)
{
    self.removeObject(object);
}

/// Implement the addObject function.
py::object addObjectKwargs(Node* self, const std::string& type, const py::kwargs& kwargs)
{
    if (kwargs.contains("name"))
    {
        std::string name = py::str(kwargs["name"]);
        if (sofapython3::isProtectedKeyword(name))
            throw py::value_error("addObject: Cannot call addObject with name " + name + ": Protected keyword");
    }
    /// Prepare the description to hold the different python attributes as data field's
    /// arguments then create the object.
    BaseObjectDescription desc {type.c_str(), type.c_str()};
    fillBaseObjectdescription(desc, kwargs);
    auto object = ObjectFactory::getInstance()->createObject(self, &desc);

    /// After calling createObject the returned value can be either a nullptr
    /// or non-null but with error message or non-null.
    /// Let's first handle the case when the returned pointer is null.
    if(!object)
    {
        std::stringstream tmp ;
        for(auto& s : desc.getErrors())
            tmp << s << msgendl ;
        throw py::value_error(tmp.str());
    }

    checkParamUsage(desc);

    /// Convert the logged messages in the object's internal logging into python exception.
    /// this is not a very fast way to do that...but well...python is slow anyway. And serious
    /// error management has a very high priority. If performance becomes an issue we will fix it
    /// when needed.
    if(object->countLoggedMessages({Message::Error}))
    {
        throw py::value_error(object->getLoggedMessagesAsString({Message::Error}));
    }

    for(auto a : kwargs)
    {
        BaseData* d = object->findData(py::cast<std::string>(a.first));
        if(d)
            d->setPersistent(true);
    }
    return PythonFactory::toPython(object.get());
}

/// Implement the addObject function.
py::object addKwargs(Node* self, const py::object& callable, const py::kwargs& kwargs)
{
    if(py::isinstance<BaseObject*>(callable))
    {
        BaseObject* obj = py::cast<BaseObject*>(callable);

        self->addObject(obj);    
        return py::cast(obj);
    }

    if(py::isinstance<Node*>(callable))
    {
        Node* node = py::cast<Node*>(callable);
        self->addChild(node);
        return py::cast(node);
    }

    if(py::isinstance<py::str>(callable))
    {
        py::str type = callable;
        return addObjectKwargs(self, type, kwargs);
    }

    if (kwargs.contains("name"))
    {
        std::string name = py::str(kwargs["name"]);
        if (sofapython3::isProtectedKeyword(name))
            throw py::value_error("addObject: Cannot call addObject with name " + name + ": Protected keyword");
    }

    auto c = callable(self, **kwargs);
    Base* base = py::cast<Base*>(c);
    for(auto a : kwargs)
    {
        BaseData* d = base->findData(py::cast<std::string>(a.first));
        if(d)
            d->setPersistent(true);
    }

    return c;
}


/// Only addObject is needed now, the createObject is deprecated and will prints
/// a warning for old scenes.
py::object createObject(Node* self, const std::string& type, const py::kwargs& kwargs)
{
    msg_deprecated(self) << "The Node.createObject method is deprecated since sofa 19.06."
                            "To remove this warning message, use 'addObject'.";
    return addObjectKwargs(self, type,kwargs);
}

py::object addChildKwargs(Node* self, const std::string& name, const py::kwargs& kwargs)
{
    if (sofapython3::isProtectedKeyword(name))
        throw py::value_error("addChild: Cannot call addChild with name " + name + ": Protected keyword");
    BaseObjectDescription desc (name.c_str());
    fillBaseObjectdescription(desc,kwargs);
    auto node=simpleapi::createChild(self, desc);
    checkParamUsage(desc);

    for(auto a : kwargs)
    {
        BaseData* d = node->findData(py::cast<std::string>(a.first));
        if(d)
            d->setPersistent(true);
    }

    return py::cast(node);
}


/// Only addChild is needed now, the createChild is deprecated and will prints a warning for old scenes.
Node* addChild(Node* self, Node* child)
{
    self->addChild(child);
    return child;
}

/// deprecated, use addChild instead. Keeping for compatibility reasons
py::object createChild(Node* self, const std::string& name, const py::kwargs& kwargs)
{
    msg_deprecated(self) << "The Node.createChild method is deprecated since sofa 19.06."
                            "To remove this warning message, use 'addChild'.";
    return addChildKwargs(self, name, kwargs);
}

py::object getChild(Node &n, const std::string &name)
{
    Node *child = n.getChild(name);
    if (child)
        return py::cast(child);
    return py::none();
}

/// Methods: removeChild / removeChildByName
/// Examples:
///     node1.removeChild(node2)
///     node1.removeChild("nodename")
void removeChild(Node& self, Node& n) { self.removeChild(&n); }
py::object removeChildByName(Node& n, const std::string name)
{
    Node* node = n.getChild(name);
    if(node==nullptr)
        throw py::index_error("Invalid name '"+name+"'");

    n.removeChild(node);
    return py::cast(node);
}

NodeIterator* property_children(Node* node)
{
    return new NodeIterator(node, [](Node* n) -> size_t { return n->child.size(); },
    [](Node* n, unsigned int index) -> Base::SPtr { return n->child[index]; });
}

NodeIterator* property_parents(Node* node)
{
    return new NodeIterator(node, [](Node* n) -> size_t { return n->getNbParents(); },
    [](Node* n, unsigned int index) -> Node::SPtr {
    auto p = n->getParents();
    return static_cast<Node*>(p[index]);
});
}

NodeIterator* property_objects(Node* node)
{
    return new NodeIterator(node, [](Node* n) -> size_t { return n->object.size(); },
    [](Node* n, unsigned int index) -> Base::SPtr { return (n->object[index]);
});
}

py::object __getattr__(Node& self, const std::string& name)
{
    /// Search in the object lists
    BaseObject *object = self.getObject(name);
    if (object)
        return PythonFactory::toPython(object);

    /// Search in the child lists
    Node *child = self.getChild(name);
    if (child)
        return PythonFactory::toPython(child);

    /// Search in the data & link lists
    return BindingBase::GetAttr(&self, name, true);
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
py::object __getitem__(Node& self, const std::string& s)
{
    std::list<std::string> stringlist;
    std::istringstream iss(s);
    std::string token;
    while (std::getline(iss, token, '.')) {
        if (!token.empty())
            stringlist.push_back(token);
    }

    if(stringlist.empty())
        throw py::value_error("Invalid path provided");

    // perform here the syntax checks over the string to parse
    // special case allowed: root[".attr1"]
    if (stringlist.front().empty()) stringlist.pop_front();
    for (const auto& string : stringlist)
        if (string.empty())
            throw py::value_error("Invalid path provided");
    // finally search for the object at the given path:
    return getItem(self, stringlist);

}

void moveChild(Node *self, BaseNode* child, BaseNode* prevParent)
{
    if (!child || !prevParent)
        throw py::value_error("Invalid arguments provided");
    self->moveChild(child, prevParent);
}

py::object getMass(Node *self)
{
    sofa::core::behavior::BaseMass* mass = self->mass.get();
    if (mass) {
        return PythonFactory::toPython(mass);
    }
    return py::none();
}


py::object getForceField(Node *self, unsigned int index)
{
    sofa::core::behavior::BaseForceField* ff = self->forceField.get(index);
    if (ff) {
        return PythonFactory::toPython(ff);
    }
    return py::none();
}


py::object getMechanicalState(Node *self)
{
    sofa::core::behavior::BaseMechanicalState* state = self->mechanicalState.get();
    if (state) {
        return PythonFactory::toPython(state);
    }
    return py::none();
}


py::object getMechanicalMapping(Node *self)
{
    sofa::core::BaseMapping* mapping = self->mechanicalMapping.get();
    if (mapping) {
        return PythonFactory::toPython(mapping);
    }
    return py::none();
}

py::object getRoot(Node* self)
{
    auto root = self->getRoot();
    if (root) {
        return PythonFactory::toPython(root);
    }
    return py::none();
}

void sendEvent(Node* self, py::object pyUserData, char* eventName)
{
    sofapython3::PythonScriptEvent event(self, eventName, pyUserData);
    self->propagateEvent(sofa::core::ExecParams::defaultInstance(), &event);
}

void moduleAddNode(py::module &m) {
    /// Register the complete parent-child relationship between Base and Node to the pybind11
    /// typing system.
    py::class_<sofa::core::objectmodel::BaseNode,
            sofa::core::objectmodel::Base,
            py_shared_ptr<sofa::core::objectmodel::BaseNode>>(m, "BaseNode");

    py::class_<Node, sofa::core::objectmodel::BaseNode,
            sofa::core::objectmodel::Context, py_shared_ptr<Node>>
            p(m, "Node", sofapython3::doc::sofa::core::Node::Class);

    PythonFactory::registerType<sofa::simulation::graph::DAGNode>(
                [](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<Node*>(object->toBaseNode()));
    });

    p.def(py::init(&__init__noname), sofapython3::doc::sofa::core::Node::init);
    p.def(py::init(&__init__), sofapython3::doc::sofa::core::Node::init1Arg, py::arg("name"));
    p.def("init", &init, sofapython3::doc::sofa::core::Node::initSofa );
    p.def("add", &addKwargs, sofapython3::doc::sofa::core::Node::addKwargs);
    p.def("addObject", &addObjectKwargs, sofapython3::doc::sofa::core::Node::addObjectKwargs);
    p.def("addObject", &addObject, sofapython3::doc::sofa::core::Node::addObject);
    p.def("createObject", &createObject, sofapython3::doc::sofa::core::Node::createObject);
    p.def("addChild", &addChildKwargs, sofapython3::doc::sofa::core::Node::addChildKwargs);
    p.def("addChild", &addChild, sofapython3::doc::sofa::core::Node::addChild);
    p.def("createChild", &createChild, sofapython3::doc::sofa::core::Node::createChild);
    p.def("getChild", &getChild, sofapython3::doc::sofa::core::Node::getChild);
    p.def("removeChild", &removeChild, sofapython3::doc::sofa::core::Node::removeChild);
    p.def("removeChild", &removeChildByName, sofapython3::doc::sofa::core::Node::removeChildWithName);
    p.def("getRoot", &getRoot, sofapython3::doc::sofa::core::Node::getRoot);
    p.def("getPathName", &Node::getPathName, sofapython3::doc::sofa::core::Node::getPathName);
    p.def("getLinkPath", &getLinkPath, sofapython3::doc::sofa::core::Node::getLinkPath);
    p.def_property_readonly("children", &property_children, sofapython3::doc::sofa::core::Node::children);
    p.def_property_readonly("parents", &property_parents, sofapython3::doc::sofa::core::Node::parents);
    p.def_property_readonly("objects", &property_objects, sofapython3::doc::sofa::core::Node::objects);
    p.def("__getattr__", &__getattr__);
    p.def("__getitem__", &__getitem__);
    p.def("removeObject", &removeObject, sofapython3::doc::sofa::core::Node::removeObject);
    p.def("getRootPath", &Node::getRootPath, sofapython3::doc::sofa::core::Node::getRootPath);
    p.def("moveChild", &moveChild, sofapython3::doc::sofa::core::Node::moveChild);
    p.def("isInitialized", &Node::isInitialized, sofapython3::doc::sofa::core::Node::isInitialized);
    p.def("getAsACreateObjectParameter", &getLinkPath, sofapython3::doc::sofa::core::Node::getAsACreateObjectParameter);
    p.def("detachFromGraph", &Node::detachFromGraph, sofapython3::doc::sofa::core::Node::detachFromGraph);
    p.def("getMass", &getMass, sofapython3::doc::sofa::core::Node::getMass);
    p.def("getForceField", &getForceField, sofapython3::doc::sofa::core::Node::getForceField);
    p.def("getMechanicalState", &getMechanicalState, sofapython3::doc::sofa::core::Node::getMechanicalState);
    p.def("getMechanicalMapping", &getMechanicalMapping, sofapython3::doc::sofa::core::Node::getMechanicalMapping);
    p.def("sendEvent", &sendEvent, sofapython3::doc::sofa::core::Node::sendEvent);

}
} /// namespace sofapython3

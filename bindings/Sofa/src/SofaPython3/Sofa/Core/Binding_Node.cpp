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
/// Neede to have automatic conversion from pybind types to stl container.
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <sofa/simulation/Simulation.h>
#include <sofa/simulation/mechanicalvisitor/MechanicalComputeEnergyVisitor.h>
#include <sofa/core/ComponentNameHelper.h>

#include <sofa/core/objectmodel/BaseData.h>
using sofa::core::objectmodel::BaseData;

#include <sofa/simpleapi/SimpleApi.h>
namespace simpleapi = sofa::simpleapi;

#include <sofa/helper/logging/Messaging.h>
using sofa::helper::logging::Message;

#include <sofa/helper/DiffLib.h>
using sofa::helper::getClosestMatch;

#include <sofa/simulation/graph/DAGNode.h>
using sofa::core::ExecParams;

#include <SofaPython3/LinkPath.h>
using sofapython3::LinkPath;

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_BaseObject.h>
#include <SofaPython3/DataHelper.h>

#include <sofa/core/ObjectFactory.h>
using sofa::core::ObjectFactory;

#include <SofaPython3/PythonFactory.h>
using sofapython3::PythonFactory;

#include <SofaPython3/PythonEnvironment.h>
using sofapython3::PythonEnvironment;

#include <SofaPython3/Sofa/Core/Binding_Node.h>
#include <SofaPython3/Sofa/Core/Binding_Node_doc.h>
#include <SofaPython3/Sofa/Core/Binding_NodeIterator.h>
#include <SofaPython3/Sofa/Core/Binding_PythonScriptEvent.h>

#include <SofaPython3/SpellingSuggestionHelper.h>

using sofa::core::objectmodel::BaseObjectDescription;

#include <queue>
#include <sofa/core/objectmodel/Link.h>

// These two lines are there to handle deprecated version of pybind.
SOFAPYTHON3_BIND_ATTRIBUTE_ERROR()
SOFAPYTHON3_ADD_PYBIND_TYPE_FOR_OLD_VERSION()

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

using sofa::simulation::Node;

namespace sofapython3
{

namespace
{
bool checkParamUsage(BaseObjectDescription& desc, const Base* base)
{
    std::vector<std::tuple<std::string, std::string>> paramErrors;
    for( auto& it : desc.getAttributeMap() )
    {
        if (!it.second.isAccessed())
        {
            paramErrors.emplace_back(std::make_tuple(it.first, it.second));
        }
    }

    if(!paramErrors.empty() || !desc.getErrors().empty())
    {
        std::stringstream tmp;
        tmp << "Unknown Attribute(s): " << msgendl;

        std::vector<std::string> possibleNames;
        if(base)
        {
            fillVectorOfStringFrom(base->getDataFields(), std::back_inserter(possibleNames), [](const BaseData* d){return d->getName();});
            fillVectorOfStringFrom(base->getLinks(), std::back_inserter(possibleNames), [](const BaseLink* l){return l->getName();});
        }

        for(auto& [name, value] : paramErrors)
        {
            tmp << "  - Unable to set attribute '"<< name <<"' with value: " << value;
            const auto& v = getClosestMatch(name, possibleNames);
            if(!v.empty())
                tmp << ". Possible misspelling of attribute '" << std::get<0>(v[0]) << "' ?";
            else
                tmp << ".";
            tmp << msgendl;
        }

        if(!desc.getErrors().empty())
            tmp << desc.getErrors()[0];
        throw py::type_error(tmp.str());
    }

    return false;
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
    return std::move(dag_node);
}

py_shared_ptr<Node> __init__(const std::string& name) {
    auto dag_node = sofa::core::objectmodel::New<sofa::simulation::graph::DAGNode>(name);
    return std::move(dag_node);
}

/// Method: init (beware this is not the python __init__, this is sofa's init())
void init(Node& self) { self.init(sofa::core::execparams::defaultInstance()); }

py::object addObject(Node& self, BaseObject * object)
{
    try {
        if (self.addObject(object))
            return PythonFactory::toPython(object);
    } catch (...) {
        throw py::type_error("Trying to add an object that isn't derived from sofa::core::objectmodel::BaseObject.");
    }
    return py::none();
}

void removeObject(Node& self, BaseObject* object)
{
    self.removeObject(object);
}

py::object hasObject(Node &n, const std::string &name)
{
    BaseObject *object = n.getObject(name);
    if (object)
        return py::cast(true);
    return py::cast(false);
}

py::object getObject(Node &n, const std::string &name, const py::kwargs& kwargs)
{
    if(kwargs.size()!=0)
    {
        msg_deprecated(&n) << "Calling the method getObject() with extra arguments is not supported anymore."
                           << "To remove this message please refer to the documentation of the getObject method"
                           << msgendl
                           << PythonEnvironment::getPythonCallingPointString() ;
    }

    BaseObject *object = n.getObject(name);
    if (object)
        return PythonFactory::toPython(object);
    return py::none();
}

void setFieldsFromPythonValues(Base* self, const py::kwargs& dict)
{
    // For each argument of the addObject function we check if this is an argument we can do a raw conversion from.
    // Doing a raw conversion means that we are not converting the argument anymore into a sofa parsable string.
    for(auto [key, value] : dict)
    {
        if(py::isinstance<LinkPath>(value))
        {
            auto* data = self->findData(py::str(key));
            if(data)
                BindingBase::SetData(data, py::cast<py::object>(value));

            auto* link = self->findLink(py::str(key));
            if(link)
                BindingBase::SetLink(link, py::cast<py::object>(value));
        }
    }
}

class NumpyReprFixerRAII
{
public:
    NumpyReprFixerRAII()
    {
        using namespace pybind11::literals;

        m_numpy = py::module_::import("numpy");
        const std::string version = py::cast<std::string>(m_numpy.attr("__version__"));
        m_majorVersion = std::stoi(version.substr(0,1));
        if ( m_majorVersion > 1)
        {
            m_setPO =  m_numpy.attr("set_printoptions");
            m_initialState = m_numpy.attr("get_printoptions")();
            m_setPO("legacy"_a = "1.25");
        }
    }

    ~NumpyReprFixerRAII()
    {
        if ( m_majorVersion > 1)
        {
            m_setPO(**m_initialState);
        }
    }

private:
    py::module_ m_numpy;
    int m_majorVersion;
    py::object m_setPO;
    py::dict m_initialState;

};


/// Implement the addObject function.
py::object addObjectKwargs(Node* self, const std::string& type, const py::kwargs& kwargs)
{
    //Instantiating this object will make sure the numpy representation is fixed during the call of this function, and comes back to its previous state after
    [[maybe_unused]] const NumpyReprFixerRAII numpyReprFixer;

    std::string name {};
    if (kwargs.contains("name"))
    {
        name = py::str(kwargs["name"]);
        if (sofapython3::isProtectedKeyword(name))
            throw py::value_error("Cannot call addObject with name " + name + ": Protected keyword");
    }
    // Prepare the description to hold the different python attributes as data field's
    // arguments then create the object.
    BaseObjectDescription desc {nullptr, type.c_str()};
    fillBaseObjectdescription(desc, kwargs);
    auto object = ObjectFactory::getInstance()->createObject(self, &desc);

    // After calling createObject the returned value can be either a nullptr
    // or non-null but with error message or non-null.
    // Let's first handle the case when the returned pointer is null.
    if(!object)
    {
        std::stringstream tmp ;
        for(auto& s : desc.getErrors())
            tmp << s << msgendl ;
        throw py::value_error(tmp.str());
    }

    // Associates the emission location to the created object.
    auto finfo = PythonEnvironment::getPythonCallingPointAsFileInfo();
    object->setInstanciationSourceFileName(finfo->filename);
    object->setInstanciationSourceFilePos(finfo->line);

    if (name.empty())
    {
        const auto resolvedName = self->getNameHelper().resolveName(object->getClassName(), name, sofa::core::ComponentNameHelper::Convention::python);
        object->setName(resolvedName);
    }

    setFieldsFromPythonValues(object.get(), kwargs);

    checkParamUsage(desc, object.get());

    // Convert the logged messages in the object's internal logging into python exception.
    // this is not a very fast way to do that...but well...python is slow anyway. And serious
    // error management has a very high priority. If performance becomes an issue we will fix it
    // when needed.
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
            throw py::value_error("add: Cannot call addObject with name " + name + ": Protected keyword");
    }

    auto c = callable(self, **kwargs);
    Base* base = py::cast<Base*>(c);
    if(!py::isinstance<Base*>(c))
    {
        throw py::value_error("add: the function passed as first argument can only return a Sofa.BaseObject or Sofa.Node object");
    }

    // Set the creation point
    auto finfo = PythonEnvironment::getPythonCallingPointAsFileInfo();
    base->setInstanciationSourceFileName(finfo->filename);
    base->setInstanciationSourceFilePos(finfo->line);

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
    msg_deprecated(self) << "The Node.createObject method is deprecated since sofa 19.06." << msgendl
                         << "To remove this warning message, use 'addObject' instead of 'createObject'." <<  msgendl
                         << msgendl
                         << PythonEnvironment::getPythonCallingPointString() ;
    return addObjectKwargs(self, type,kwargs);
}

py::object addChildKwargs(Node* self, const std::string& name, const py::kwargs& kwargs)
{
    //Instantiating this object will make sure the numpy representation is fixed during the call of this function, and comes back to its previous state after
    [[maybe_unused]] const NumpyReprFixerRAII numpyReprFixer;

    if (sofapython3::isProtectedKeyword(name))
        throw py::value_error("addChild: Cannot call addChild with name " + name + ": Protected keyword");
    BaseObjectDescription desc (name.c_str());
    fillBaseObjectdescription(desc,kwargs);
    auto node=simpleapi::createChild(self, desc);
    auto finfo = PythonEnvironment::getPythonCallingPointAsFileInfo();
    node->setInstanciationSourceFileName(finfo->filename);
    node->setInstanciationSourceFilePos(finfo->line);

    checkParamUsage(desc, node.get());

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
    msg_deprecated(self) << "The Node.createChild method is deprecated since sofa 19.06." << msgendl
                         << "To remove this warning message, use 'addChild' instead of 'createChild'." << msgendl
                         << msgendl
                         << PythonEnvironment::getPythonCallingPointString() ;
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

std::unique_ptr<NodeIterator> property_children(Node* node)
{
    return std::make_unique<NodeIterator>(node,
                                          [](Node* n) -> size_t { return n->child.size(); },
    [](Node* n, unsigned int index) -> Base::SPtr { return n->child[index]; },
[](const Node* n, const std::string& name) { return n->getChild(name); },
[](Node* n, unsigned int index) { n->removeChild(n->child[index]); }
);
}

std::unique_ptr<NodeIterator> property_parents(Node* node)
{
    return std::make_unique<NodeIterator>(node,
                                          [](Node* n) -> size_t { return n->getNbParents(); },
    [](Node* n, unsigned int index) -> Node::SPtr {
    auto p = n->getParents();
    return static_cast<Node*>(p[index]);
},
[](const Node* n, const std::string& name) -> sofa::core::Base* {
    const auto& parents = n->getParents();
    return *std::find_if(parents.begin(),
                         parents.end(),
                         [name](BaseNode* child){ return child->getName() == name; });
},
[](Node*, unsigned int) {
    throw std::runtime_error("Removing a parent is not a supported operation. Please detach the node from the corresponding graph node.");
});
}

std::unique_ptr<NodeIterator> property_objects(Node* node)
{
    return std::make_unique<NodeIterator>(node,
                                          [](Node* n) -> size_t { return n->object.size(); },
    [](Node* n, unsigned int index) -> Base::SPtr { return (n->object[index]);},
[](const Node* n, const std::string& name) { return n->getObject(name); },
[](Node* n, unsigned int index) { n->removeObject(n->object[index]);}
);
}

py::object __getattr__(py::object pyself, const std::string& name)
{
    Node* selfnode = py::cast<Node*>(pyself);
    /// Search in the object lists
    BaseObject *object = selfnode->getObject(name);
    if (object)
        return PythonFactory::toPython(object);

    /// Search in the child lists
    Node *child = selfnode->getChild(name);
    if (child)
        return PythonFactory::toPython(child);

    /// Search in the data & link lists
    py::object result = BindingBase::GetAttr(selfnode, name, false);
    if(!result.is_none())
        return result;

    std::stringstream tmp;
    emitSpellingMessage(tmp, "   - The data field named ", selfnode->getDataFields(), name, 2, 0.8);
    emitSpellingMessage(tmp, "   - The link named ", selfnode->getDataFields(), name, 2, 0.8);
    emitSpellingMessage(tmp, "   - The object named ", selfnode->getNodeObjects(), name, 2, 0.8);
    emitSpellingMessage(tmp, "   - The child node named ", selfnode->getChildren(), name, 2, 0.8);

    // Also provide spelling hints on python functions.
    emitSpellingMessage(tmp, "   - The python attribute named ", py::cast<py::dict>(py::type::of(pyself).attr("__dict__")), name, 5, 0.8,
                        [](const std::pair<py::handle, py::handle>& kv) { return py::cast<std::string>(std::get<0>(kv)); });

    std::stringstream message;
    message << "Unable to find attribute: "+name;
    if(!tmp.str().empty())
    {
        message << msgendl;
        message << "   You possibly wanted to access: " << msgendl;
        message << tmp.rdbuf();
    }
    throw pybind11::attribute_error(message.str());
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
        return PythonFactory::toPython(sofa::core::castToBase(mass));
    }
    return py::none();
}


py::object getForceField(Node *self, unsigned int index)
{
    sofa::core::behavior::BaseForceField* ff = self->forceField.get(index);
    if (ff) {
        return PythonFactory::toPython(sofa::core::castToBase(ff));
    }
    return py::none();
}


py::object getMechanicalState(Node *self)
{
    sofa::core::behavior::BaseMechanicalState* state = self->mechanicalState.get();
    if (state) {
        return PythonFactory::toPython(sofa::core::castToBase(state));
    }
    return py::none();
}


py::object hasODESolver(Node *self)
{
    const bool hasODE = self->solver.size() > 0;
    return py::cast(hasODE);
}


py::object getMechanicalMapping(Node *self)
{
    sofa::core::BaseMapping* mapping = self->mechanicalMapping.get();
    if (mapping) {
        return PythonFactory::toPython(sofa::core::castToBase(mapping));
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
    self->propagateEvent(sofa::core::execparams::defaultInstance(), &event);
}

py::object computeEnergy(Node* self)
{
    sofa::simulation::mechanicalvisitor::MechanicalComputeEnergyVisitor energyVisitor(sofa::core::mechanicalparams::defaultInstance());
    energyVisitor.execute(self->getContext());
    const SReal kineticEnergy = energyVisitor.getKineticEnergy();
    const SReal potentialEnergy = energyVisitor.getPotentialEnergy();
    return py::cast(std::make_tuple(kineticEnergy, potentialEnergy));
}

}

void moduleAddNode(py::module &m) {
    /// Register the complete parent-child relationship between Base and Node to the pybind11
    /// typing system.
    py::class_<sofa::core::objectmodel::BaseNode,
            sofa::core::objectmodel::Base,
            py_shared_ptr<sofa::core::objectmodel::BaseNode>>(m, "BaseNode", "Base class for simulation node");

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
    p.def("addObject", &addObject, sofapython3::doc::sofa::core::Node::addObject, py::keep_alive<0, 2>());
    p.def("createObject", &createObject, sofapython3::doc::sofa::core::Node::createObject, py::keep_alive<0, 2>());
    p.def("hasObject", &hasObject, sofapython3::doc::sofa::core::Node::hasObject);
    p.def("getObject", &getObject, sofapython3::doc::sofa::core::Node::getObject);
    p.def("addChild", &addChildKwargs, sofapython3::doc::sofa::core::Node::addChildKwargs);
    p.def("addChild", &addChild, sofapython3::doc::sofa::core::Node::addChild, py::keep_alive<0, 2>());
    p.def("createChild", &createChild, sofapython3::doc::sofa::core::Node::createChild, py::keep_alive<0, 2>());
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
    p.def("hasODESolver", &hasODESolver, sofapython3::doc::sofa::core::Node::hasODESolver);
    p.def("getForceField", &getForceField, sofapython3::doc::sofa::core::Node::getForceField);
    p.def("getMechanicalState", &getMechanicalState, sofapython3::doc::sofa::core::Node::getMechanicalState);
    p.def("getMechanicalMapping", &getMechanicalMapping, sofapython3::doc::sofa::core::Node::getMechanicalMapping);
    p.def("sendEvent", &sendEvent, sofapython3::doc::sofa::core::Node::sendEvent);
    p.def("computeEnergy", &computeEnergy, sofapython3::doc::sofa::core::Node::computeEnergy);

}
} /// namespace sofapython3

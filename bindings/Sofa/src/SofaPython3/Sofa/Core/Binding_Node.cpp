
/// Neede to have automatic conversion from pybind types to stl container.
#include <pybind11/stl.h>

#include <sofa/core/objectmodel/BaseData.h>
#include <sofa/simulation/Simulation.h>
using sofa::core::objectmodel::BaseData;

#include <SofaSimulationGraph/SimpleApi.h>
namespace simpleapi = sofa::simpleapi;

#include <sofa/helper/logging/Messaging.h>
using sofa::helper::logging::Message;

#include <SofaSimulationGraph/DAGSimulation.h>
#include <SofaSimulationGraph/DAGNode.h>
using sofa::core::ExecParams;

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/DataHelper.h>
#include "Binding_Node.h"

#include <sofa/core/ObjectFactory.h>
using sofa::core::ObjectFactory;

#include <SofaPython3/Sofa/Core/PythonDownCast.h>
using sofapython3::PythonDownCast;

using sofa::core::objectmodel::BaseObjectDescription;

#include <queue>
#include <sofa/core/objectmodel/Link.h>

namespace sofapython3
{

std::string toSofaParsableString(const py::handle& p)
{
    if(py::isinstance<py::list>(p) || py::isinstance<py::tuple>(p))
    {
        std::stringstream tmp;
        for(auto pa : p){
            tmp << toSofaParsableString(pa) << " ";
        }
        return tmp.str();
    }
    //TODO(dmarchal) This conversion to string is so bad.
    if(py::isinstance<py::str>(p))
        return py::str(p);
    return py::repr(p);
}

/// RVO optimized function. Don't care about copy on the return code.
void fillBaseObjectdescription(BaseObjectDescription& desc, const py::dict& dict)
{
    for(auto kv : dict)
    {
        desc.setAttribute(py::str(kv.first), toSofaParsableString(kv.second));
    }
}

bool checkParamUsage(Base* object, BaseObjectDescription& desc)
{
    bool hasFailure = false;
    std::stringstream tmp;
    tmp <<"Unknown Attribute(s): " << msgendl;
    for( auto it : desc.getAttributeMap() )
    {
        if (!it.second.isAccessed())
        {
            hasFailure = true;
            tmp << " - \""<<it.first <<"\" with value: \"" <<(std::string)it.second << msgendl;
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

void moduleAddBaseIterator(py::module &m)
{
    py::class_<BaseIterator> d(m, "NodeIterator");

    d.def("__getitem__", [](BaseIterator& d, size_t index) -> py::object
    {
        if(index>=d.size(d.owner.get()))
            throw py::index_error("Too large index '"+std::to_string(index)+"'");
        return py::cast(d.get(d.owner.get(), index));
    });

    d.def("__getitem__", [](BaseIterator& d, const std::string& name) -> py::object
    {
        BaseObject* obj =d.owner->getObject(name);
        if(obj==nullptr)
            throw py::index_error("Not existing object '"+name+"'");
        return py::cast(obj);
    });

    d.def("__iter__", [](BaseIterator& d)
    {
        return d;
    });
    d.def("__next__", [](BaseIterator& d) -> py::object
    {
        if(d.index>=d.size(d.owner.get()))
            throw py::stop_iteration();
        return py::cast(d.get(d.owner.get(), d.index++));
    });
    d.def("__len__", [](BaseIterator& d) -> py::object
    {
        return py::cast(d.size(d.owner.get()));
    });

    d.def("at", [](BaseIterator& d, size_t index) -> py::object
    {
        return py::cast(d.get(d.owner.get(), index));
    });
    d.def("remove_at", [](BaseIterator& d, size_t index)
    {
        BaseNode::SPtr n(dynamic_cast<BaseNode*>(d.get(d.owner.get(), index).get()));
        d.owner->removeChild(n);
    });
}

py::object Node_addChild(Node* self, const std::string& name, const py::kwargs& kwargs)
{
    if (sofapython3::isProtectedKeyword(name))
        throw py::value_error("addChild: Cannot call addChild with name " + name + ": Protected keyword");
    BaseObjectDescription desc (name.c_str());
    fillBaseObjectdescription(desc,kwargs);
    auto node=simpleapi::createChild(self, desc);
    checkParamUsage(node.get(), desc);
    return py::cast(node);
}

/// Implement the addObject function.
py::object Node_addObject(Node* self, const std::string& type, const py::kwargs& kwargs)
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

    checkParamUsage(object.get(), desc);

    /// Convert the logged messages in the object's internal logging into python exception.
    /// this is not a very fast way to do that...but well...python is slow anyway. And serious
    /// error management has a very high priority. If performance becomes an issue we will fix it
    /// when needed.
    if(object->countLoggedMessages({Message::Error}))
    {
        throw py::value_error(object->getLoggedMessagesAsString({Message::Error}));
    }

    return py::cast(object);
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

void moduleAddNode(py::module &m) {
    //py::options options;
    //options.disable_function_signatures();

    moduleAddBaseIterator(m);

    PythonDownCast::registerType<sofa::simulation::graph::DAGNode>(
                [](sofa::core::objectmodel::Base* object)
    {
        return py::cast(static_cast<Node*>(object->toBaseNode()));
    });

    py::class_<Node, sofa::core::objectmodel::BaseNode,
            sofa::core::objectmodel::Context, Node::SPtr>
            p(m, "Node",
              R"(
              Node of the scene graph
              ---------------

              .. autoclass:: Sofa.Node
              :members:
              :undoc-members:

              )");

    /// Constructors:
    p.def(py::init([](){ return sofa::core::objectmodel::New<sofa::simulation::graph::DAGNode>("unnamed"); }),
          ":rtype: Sofa.Simulation.Node");
    p.def(py::init([](const std::string& name){
        return sofa::core::objectmodel::New<sofa::simulation::graph::DAGNode>(name);
    }), ":rtype: Sofa.Simulation.Node", py::arg("name"));

    /// Method: init (beware this is not the python __init__, this is sofa's init())
    p.def("init", [](Node& self) { self.init(ExecParams::defaultInstance()); } );

    /// Method: addObjects
    /// Only addObject is needed now, the createObject is deprecated and will prints
    /// a warning for old scenes.
    p.def("addObject", &Node_addObject, "Hello add object...");
    p.def("addObject", [](Node& self, BaseObject* object) -> py::object
    {
        if(self.addObject(object))
            return PythonDownCast::toPython(object);
        return py::none();
    });
    p.def("createObject",
          [](Node* self, const std::string& type, const py::kwargs& kwargs) {
        msg_deprecated(self) << "The Node.createObject method is deprecated since sofa 19.06."
                                "To remove this warning message, use 'addObject'.";
        return Node_addObject(self, type,kwargs);

    });

    /// Method: addChild.
    /// Only addChild is needed now, the createChild is deprecated and will prints a warning for old scenes.
    p.def("addChild", &Node_addChild, ":rtype: Sofa.Simulation.Node");
    p.def("addChild", [](Node* self, Node* child)
    {
        self->addChild(child);
        return child;
    }, ":rtype: Sofa.Simulation.Node");
    p.def("createChild", [](Node* self, const std::string& name, const py::kwargs& kwargs)
    {
        msg_deprecated(self) << "The Node.createChild method is deprecated since sofa 19.06."
                                "To remove this warning message, use 'addChild'.";
        return Node_addChild(self, name, kwargs);
    });


    /// Method: getChild.
    p.def("getChild", [](Node &n, const std::string &name) -> py::object
    {
        Node *child = n.getChild(name);
        if (child)
            return py::cast(child);
        return py::none();
    });

    /// Methods: removeChild
    /// Examples:
    ///     node1.removeChild(node2)
    ///     node1.removeChild("nodename")
    p.def("removeChild", [](Node& self, Node& n){ self.removeChild(&n); });
    p.def("removeChild", [](Node& n, const std::string name)
    {
        Node* node = n.getChild(name);
        if(node==nullptr)
            throw py::index_error("Invalid name '"+name+"'");

        n.removeChild(node);
        return py::cast(node);
    });

    p.def("getRoot", &Node::getRoot);
    p.def("getPathName", &Node::getPathName);
    p.def("getLink", [](Node* node) {
        return ("@"+node->getPathName()).c_str();
    });

    p.def_property_readonly("children", [](Node* node)
    {
        return new BaseIterator(node, [](Node* n) -> size_t { return n->child.size(); },
        [](Node* n, unsigned int index) -> Node::SPtr { return n->child[index]; });
});

p.def_property_readonly("parents", [](Node* node)
{
    return new BaseIterator(node,
                            [](Node* n) -> size_t { return n->getNbParents(); },
    [](Node* n, unsigned int index) -> Node::SPtr {
    auto p = n->getParents();
    return static_cast<Node*>(p[index]);
});
});

p.def_property_readonly("objects", [](Node* node)
{
    return new BaseIterator(node,
                            [](Node* n) -> size_t { return n->object.size(); },
    [](Node* n, unsigned int index) -> Base::SPtr {
    return (n->object[index]);
});
});

p.def("__getattr__", [](Node& self, const std::string& name) -> py::object
{
    /// Search in the object lists
    BaseObject *object = self.getObject(name);
    if (object)
        return PythonDownCast::toPython(object);

    /// Search in the child lists
    Node *child = self.getChild(name);
    if (child)
        return PythonDownCast::toPython(child);

    /// Search in the data & link lists
    return BindingBase::GetAttr(&self, name, true);
});



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
p.def("__getitem__", [](Node& self, const std::string& s) -> py::object
{
    std::list<std::string> stringlist;
    std::istringstream iss(s);
    std::string token;
    while (std::getline(iss, token, '.')) {
        if (!token.empty())
            stringlist.push_back(token);
    }

    // perform here the syntax checks over the string to parse

    // special case allowed: root[".attr1"]
    if (stringlist.front().empty()) stringlist.pop_front();
    for (const auto& string : stringlist)
        if (string.empty())
            throw py::value_error("Invalid path provided");
    // finally search for the object at the given path:
    return getItem(self, stringlist);

});


p.def("__old_getChildren", [](Node& node)
{
    py::list l;
    for(auto& child : node.child)
        l.append( py::cast(child) );
    return l;
});

p.def("__old_getChild", [](Node& node, unsigned int t)
{
    if(t >= node.child.size())
        throw py::index_error("Index trop grand");
    return py::cast(node.child[t]);
});
}

} /// namespace sofapython3

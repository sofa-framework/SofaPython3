
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

using sofa::core::objectmodel::BaseObjectDescription;

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
    BaseObjectDescription desc (name.c_str());
    fillBaseObjectdescription(desc,kwargs);
    auto node=simpleapi::createChild(self, desc);
    checkParamUsage(node.get(), desc);
    return py::cast(node);
}

/// Implement the addObject function.
py::object Node_addObject(Node* self, const std::string& type, const py::kwargs& kwargs)
{
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

void moduleAddNode(py::module &m) {
    py::options options;
    options.disable_function_signatures();

    moduleAddBaseIterator(m);

    py::class_<Node, sofa::core::objectmodel::BaseNode,
            sofa::core::objectmodel::Context, Node::SPtr>
            p(m, "Node",
              R"(Array with associated photographic information.

              ...

              Attributes
              ----------
              exposure : float
                  Exposure in seconds.

              Methods
              -------
              colorspace(c='rgb')
                  Represent the photo in the given colorspace.
              gamma(n=1.0)
                  Change the photo's gamma exposure.

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
    p.def("addObject", &Node_addObject);
    p.def("addObject", [](Node& self, BaseObject* object) -> py::object
    {
        if(self.addObject(object))
            return py::cast(object);
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
        return py::cast(object);

    /// Search in the child lists
    Node *child = self.getChild(name);
    if (child)
        return py::cast(child);

    /// Search in the data & link lists
    return BindingBase::GetAttr(&self, name, true);
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

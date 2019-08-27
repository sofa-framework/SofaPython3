#pragma once
#include <functional>
#include <pybind11/pybind11.h>
#include <SofaPython3/Sofa/Core/Binding_BaseObject.h>

#include <sofa/simulation/Node.h>

namespace sofapython3
{
namespace py { using namespace pybind11; }

using sofa::simulation::Node;
using sofa::core::objectmodel::BaseNode;

class NodeIterator
{
public:
    Node::SPtr owner;
    size_t     index=0;
    std::function<size_t (Node*)> size ;
    std::function<Base::SPtr (Node*, size_t)> get ;

    NodeIterator(Node::SPtr owner_,
                 std::function<size_t (Node*)> size_,
                 std::function<Base::SPtr (Node*, size_t)> get_)
    {
        size = size_;
        get = get_;
        owner=owner_;
        index=0;
    }
};

void moduleAddNodeIterator(py::module &m);

} /// namespace sofapython3

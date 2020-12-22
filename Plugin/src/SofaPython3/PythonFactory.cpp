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

#include <functional>
#include <SofaPython3/PythonFactory.h>
#include <SofaPython3/DataHelper.h>

#include <SofaSimulationGraph/DAGNode.h>

#include "sofa/simulation/AnimateBeginEvent.h"
using sofa::simulation::AnimateBeginEvent;
#include "sofa/simulation/AnimateEndEvent.h"
using sofa::simulation::AnimateEndEvent;
//#include "sofa/simulation/CollisionBeginEvent.h"
//#include "sofa/simulation/CollisionEndEvent.h"
//#include "sofa/simulation/IntegrateBeginEvent.h"
//#include "sofa/simulation/IntegrateEndEvent.h"
//#include "sofa/simulation/PauseEvent.h"
//#include "sofa/simulation/PositionEvent.h"
//#include "sofa/simulation/UpdateMappingEndEvent.h"

//#include "sofa/core/objectmodel/DetachNodeEvent.h"
//#include "sofa/core/objectmodel/GUIEvent.h"
//#include "sofa/core/objectmodel/HapticDeviceEvent.h"
//#include "sofa/core/objectmodel/IdleEvent.h"
//#include "sofa/core/objectmodel/JoystickEvent.h"
#include "sofa/core/objectmodel/KeypressedEvent.h"
using sofa::core::objectmodel::KeypressedEvent;
#include "sofa/core/objectmodel/KeyreleasedEvent.h"
using sofa::core::objectmodel::KeyreleasedEvent;
#include "sofa/core/objectmodel/MouseEvent.h"
using sofa::core::objectmodel::MouseEvent;
#include "sofa/core/objectmodel/ScriptEvent.h"
using sofa::core::objectmodel::ScriptEvent;
using sofa::core::objectmodel::Event;

#include <sofa/defaulttype/DataTypeInfo.h>

/// Bind the python's attribute error
namespace pybind11 { PYBIND11_RUNTIME_EXCEPTION(attribute_error, PyExc_AttributeError) }
/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

namespace sofapython3
{
using namespace pybind11::literals;

static std::map<std::string, componentDowncastingFunction> s_componentDowncastingFct;
static std::map<std::string, dataDowncastingFunction> s_dataDowncastingFct;
static std::map<std::string, eventDowncastingFunction> s_eventDowncastingFct;
static std::map<std::string, dataCreatorFunction> s_dataCreationFct;

bool PythonFactory::defaultTypesRegistered = registerDefaultTypes();
bool PythonFactory::defaultEventsRegistered = registerDefaultEvents();

void PythonFactory::registerType(const std::string& typeName, componentDowncastingFunction fct)
{
    s_componentDowncastingFct[typeName] = fct;
}

void PythonFactory::registerType(const std::string& typeName, dataDowncastingFunction fct)
{
    s_dataDowncastingFct[typeName] = fct;
}

void PythonFactory::registerType(const std::string& typeName, eventDowncastingFunction fct)
{
    s_eventDowncastingFct[typeName] = fct;
}

void PythonFactory::registerType(const std::string& typeName, dataCreatorFunction fct)
{
    s_dataCreationFct[typeName] = fct;
}

std::map<std::string, componentDowncastingFunction>::iterator PythonFactory::searchLowestCastAvailable(const sofa::core::objectmodel::BaseClass* metaclass)
{
    std::string type_name = metaclass->typeName;

    /// If there is a match with current metaclass we returns it
    auto kv = s_componentDowncastingFct.find(type_name);
    if( kv != s_componentDowncastingFct.end())
    {
        return kv;
    }

    /// If there is no match we try to find if there is a match for a parent of the requested type
    /// As this is a slow process we cache the results to speed the subsequent request.
    for(auto p : metaclass->parents)
    {
        auto kvs = searchLowestCastAvailable(p);
        if( kvs != s_componentDowncastingFct.end() )
        {
            return kvs;
        }
    }

    return s_componentDowncastingFct.end();
}

std::string get_class_hierarchy_as_string (const sofa::core::objectmodel::BaseClass* metaclass, int level = 0) {
    std::stringstream ss;
    std::string class_name = metaclass->className;
    std::string type_name = metaclass->typeName;
    ss << std::string( level*4, '-' ) << class_name << " (" << type_name << ")\n";
    for(auto p : metaclass->parents) {
        ss << get_class_hierarchy_as_string(p, level+1);
    }
    return ss.str();
}

py::object PythonFactory::toPython(sofa::core::objectmodel::Base* object)
{
    auto metaclass = object->getClass();

    /// Let's first search if there is a casting function for the give type.
    std::string type_name = metaclass->typeName;
    auto kv = s_componentDowncastingFct.find(type_name);
    if( kv != s_componentDowncastingFct.end())
    {
        return kv->second(object);
    }

    /// If the first search fail we will starting moving forward in the inheritance graph to
    /// detect which of its parents has a downcast function available. This function stops
    /// at first match.
    kv = searchLowestCastAvailable(metaclass);

    if (kv == s_componentDowncastingFct.end()) {
        msg_error("PythonFactory") << "Unable to find a python binding for an object in-heriting from Base.\n"
                                   << "Tried with:\n" << get_class_hierarchy_as_string(metaclass);
        throw std::runtime_error("Unable to find a python binding for an object in-heriting from Base.");
    }

    s_componentDowncastingFct[type_name] = kv->second;
    return kv->second(object);
}

py::object PythonFactory::toPython(const sofa::core::objectmodel::BaseData* data)
{
    return toPython(const_cast<BaseData*>(data));
}

py::object PythonFactory::toPython(sofa::core::objectmodel::BaseData* data)
{
    auto type = data->getValueTypeInfo()->name();

    /// Let's first search if there is a casting function for the given type.
    auto kv = s_dataDowncastingFct.find(type);
    if( kv != s_dataDowncastingFct.end())
    {
        return kv->second(data);
    }

    const sofa::defaulttype::AbstractTypeInfo& nfo { *(data->getValueTypeInfo()) };

    if(nfo.Container() && nfo.SimpleLayout())
    {
        s_dataDowncastingFct[type] = s_dataDowncastingFct["DataContainer"];
        return s_dataDowncastingFct[type](data);
    }
    if(nfo.Container() && nfo.Text())
    {
        s_dataDowncastingFct[type] = s_dataDowncastingFct["DataVectorString"];
        return s_dataDowncastingFct[type](data);
    }
    if(nfo.Text())
    {
        s_dataDowncastingFct[type] = s_dataDowncastingFct["DataString"];
        return s_dataDowncastingFct[type](data);
    }
    return py::cast(data);
}

py::object PythonFactory::valueToPython_ro(sofa::core::objectmodel::BaseData* data)
{
    const AbstractTypeInfo& nfo{ *(data->getValueTypeInfo()) };
    /// In case the data is a container with a simple layout
    /// we can expose the field as a numpy.array (no copy)
    if(nfo.Container() && nfo.SimpleLayout())
    {
        auto capsule = py::capsule(new Base::SPtr(data->getOwner()));
        py::buffer_info ninfo = toBufferInfo(*data);
        py::array a(pybind11::dtype(ninfo), ninfo.shape,
                    ninfo.strides, ninfo.ptr, capsule);
        a.attr("flags").attr("writeable") = false;
        return std::move(a);
    }

    /// If this is not the case we return the converted datas (copy)
    return convertToPython(data);
}

template<class SrcType>
void copyFromListOf(const AbstractTypeInfo& nfo, void* ptr, size_t index, py::object o);

template<> void copyFromListOf<double>(const AbstractTypeInfo& nfo, void* ptr, size_t index, py::object o)
{
    nfo.setScalarValue(ptr, index, py::cast<double>(o));
}

template<> void copyFromListOf<int>(const AbstractTypeInfo& nfo, void* ptr, size_t index, py::object o)
{
    nfo.setIntegerValue(ptr, index, py::cast<int>(o));
}

template<> void copyFromListOf<std::string>(const AbstractTypeInfo& nfo, void* ptr, size_t index, py::object o)
{
    nfo.setTextValue(ptr, index, py::cast<std::string>(o));
}


template<class DestType>
void copyFromListOf(BaseData& d, const AbstractTypeInfo& nfo, const py::list& l)
{
    /// Check if the data is a single dimmension or not.
    py::buffer_info dstinfo = toBufferInfo(d);

    if(dstinfo.ndim>2)
        throw py::index_error("Invalid number of dimension only 1 or 2 dimensions are supported).");

    if(dstinfo.ndim==1)
    {
        void* ptr = d.beginEditVoidPtr();
        if( size_t(dstinfo.shape[0]) != l.size())
            nfo.setSize(ptr, l.size());

        for(size_t i=0;i<l.size();++i)
        {
            copyFromListOf<DestType>(nfo, ptr, i, l[i]);
        }
        d.endEditVoidPtr();
        return;
    }
    void* ptr = d.beginEditVoidPtr();
    if( size_t(dstinfo.shape[0]) != l.size())
    {
        if( !nfo.setSize(ptr, l.size()*nfo.size()) )
            throw std::runtime_error("Unable to resize vector to match list size. Is the data type resizable ?");

        /// Update the buffer info entry to take into account the change of size.
        dstinfo = toBufferInfo(d);
    }

    for(auto i=0;i<dstinfo.shape[0];++i)
    {
        py::list ll = l[size_t(i)];
        for(auto j=0;j<dstinfo.shape[1];++j)
        {
            copyFromListOf<DestType>(nfo, ptr, size_t(i*dstinfo.shape[1]+j), ll[size_t(j)]);
        }
    }

    d.endEditVoidPtr();
    return;
}

template<>
void copyFromListOf<std::string>(BaseData& d, const AbstractTypeInfo& nfo, const py::list& l)
{
    void* ptr = d.beginEditVoidPtr();
    if( size_t(nfo.size()) != l.size())
        nfo.setSize(ptr, l.size());

    for(size_t i=0;i<l.size();++i)
    {
        copyFromListOf<std::string>(nfo, ptr, i, l[i]);
    }
    d.endEditVoidPtr();
    return;
}

void PythonFactory::fromPython(BaseData* d, const py::object& o)
{

    const AbstractTypeInfo& nfo{ *(d->getValueTypeInfo()) };

    if(!nfo.Container())
    {
        scoped_writeonly_access guard(d);
        if(nfo.Integer()) {
            nfo.setIntegerValue(guard.ptr, 0, py::cast<int>(o));
        } else if(nfo.Text()) {
            nfo.setTextValue(guard.ptr, 0, py::cast<py::str>(o));
        } else if(nfo.Scalar()) {
            nfo.setScalarValue(guard.ptr, 0, py::cast<double>(o));
        } else {
            // Custom datatype that has no DataTypeInfo. Let's see if we can convert it to a string
            if (py::isinstance<py::str>(o)) {
                d->read(py::cast<std::string>(o));
            } else if (py::isinstance<py::int_>(o)) {
                d->read(std::to_string(py::cast<int>(o)));
            } else if (py::isinstance<py::float_>(o)) {
                d->read(std::to_string(py::cast<float>(o)));
            } else if (py::isinstance<py::list>(o)) {
                const auto list = py::cast<py::list>(o);
                const std::string l = std::accumulate(list.begin(), list.end(), std::string(), [](const std::string & s, const py::handle m){
                    return s + " " + py::cast<std::string>(py::str(m));
                });
                d->read(l);
            } else {
                std::stringstream s;
                s<< "Trying to set the value of the data "
                 << d->getName() << ", but the later hasn't register it's DataTypeInfo (see sofa/defaulttype/DataTypeInfo.h for more details)";
                throw std::runtime_error(s.str());
            }
        }

        return ;
    }

    if(nfo.Integer())
        return copyFromListOf<int>(*d, nfo, o);

    if(nfo.Text())
    {
        return copyFromListOf<std::string>(*d, nfo, o);
    }

    if(nfo.Scalar())
        return copyFromListOf<double>(*d, nfo, o);

    std::stringstream s;
    s<< "binding problem, trying to set value for "
     << d->getName() << ", " << py::cast<std::string>(py::str(o));
    throw std::runtime_error(s.str());
}


py::object PythonFactory::toPython(sofa::core::objectmodel::Event* event)
{
    std::string className = event->getClassName();

    /// Let's first search if there is a casting function for the give type.
    auto kv = s_eventDowncastingFct.find(className);
    if( kv != s_eventDowncastingFct.end())
    {
        return kv->second(event);
    }

    /// If the first search fail we return a dict based on the Event*
    /// basic values (isHandled and type)
    s_eventDowncastingFct[className] =
            [](sofa::core::objectmodel::Event* event) -> py::dict {
        return py::dict("type"_a=event->getClassName(),
                        "isHandled"_a=event->isHandled());
    };

    return s_eventDowncastingFct[className](event);
}

sofa::core::objectmodel::BaseData* PythonFactory::createInstance(const std::string& typeName)
{
    auto kv = s_dataCreationFct.find(typeName);
    if( kv != s_dataCreationFct.end())
    {
        return kv->second();
    }
    return nullptr;
}

bool PythonFactory::registerDefaultEvents()
{
//    s_eventDowncastingFct = std::map<std::string, eventDowncastingFunction>();

    AnimateBeginEvent abe(.0);
    s_eventDowncastingFct[std::string(abe.getClassName())] = [] (Event* event) -> py::dict {
        auto evt = dynamic_cast<AnimateBeginEvent*>(event);
        return py::dict("type"_a=evt->getClassName(),
                        "isHandled"_a=evt->isHandled(),
                        "dt"_a=evt->getDt());
    };
    AnimateEndEvent aee(.0);
    s_eventDowncastingFct[aee.getClassName()] = [] (Event* event) -> py::dict {
        auto evt = dynamic_cast<AnimateEndEvent*>(event);
        return py::dict("type"_a=evt->getClassName(),
                        "isHandled"_a=evt->isHandled(),
                        "dt"_a=evt->getDt());
    };
    KeypressedEvent kpe('\0');
    s_eventDowncastingFct[kpe.getClassName()] = [] (Event* event) -> py::dict {
        auto evt = dynamic_cast<KeypressedEvent*>(event);
        return py::dict("type"_a=evt->getClassName(),
                        "isHandled"_a=evt->isHandled(),
                        "key"_a=evt->getKey());
    };
    KeyreleasedEvent kre('\0');
    s_eventDowncastingFct[kre.getClassName()] = [] (Event* event) -> py::dict {
        auto evt = dynamic_cast<KeyreleasedEvent*>(event);
        return py::dict("type"_a=evt->getClassName(),
                        "isHandled"_a=evt->isHandled(),
                        "key"_a=evt->getKey());
    };
    MouseEvent me(MouseEvent::State::Move);
    s_eventDowncastingFct[me.getClassName()] = [] (Event* event) -> py::dict {
        auto evt = dynamic_cast<MouseEvent*>(event);

        return py::dict("type"_a=evt->getClassName(),
                        "isHandled"_a=evt->isHandled(),
                        "State"_a=int(evt->getState()),
                        "mouseX"_a=evt->getPosX(),
                        "mouseY"_a=evt->getPosY(),
                        "wheelDelta"_a=evt->getWheelDelta());
    };
    ScriptEvent se(nullptr, "");
    s_eventDowncastingFct[se.getClassName()] = [] (Event* event) -> py::dict {
        auto evt = dynamic_cast<ScriptEvent*>(event);
        return py::dict("type"_a=evt->getClassName(),
                        "isHandled"_a=evt->isHandled(),
                        "sender"_a=(evt->getSender() ? py::cast(evt->getSender()) : py::none()),
                        "event_name"_a=evt->getEventName());
    };
    return true;
}

bool PythonFactory::registerDefaultTypes()
{
    // helper vector style containers
    std::string containers[] = {"vector"};

    // PrefabLink
    PythonFactory::registerType<sofa::core::objectmodel::PrefabLink>("PrefabLink");
    PythonFactory::registerType<sofa::core::objectmodel::PrefabLink>("Link");

    // Scalars
    PythonFactory::registerType<std::string>("string");
    PythonFactory::registerType<float>("float");
    PythonFactory::registerType<double>("double");
    PythonFactory::registerType<bool>("bool");
    PythonFactory::registerType<int>("int");

    // vectors
    PythonFactory::registerType<sofa::defaulttype::Vec2d>("Vec2d");
    PythonFactory::registerType<sofa::defaulttype::Vec3d>("Vec3d");
    PythonFactory::registerType<sofa::defaulttype::Vec4d>("Vec4d");
    PythonFactory::registerType<sofa::defaulttype::Vec6d>("Vec6d");
    PythonFactory::registerType<sofa::defaulttype::Vec2f>("Vec2f");
    PythonFactory::registerType<sofa::defaulttype::Vec3f>("Vec3f");
    PythonFactory::registerType<sofa::defaulttype::Vec4f>("Vec4f");
    PythonFactory::registerType<sofa::defaulttype::Vec6f>("Vec6f");

    // Matrices
    PythonFactory::registerType<sofa::defaulttype::Mat2x2d>("Mat2x2d");
    PythonFactory::registerType<sofa::defaulttype::Mat3x3d>("Mat3x3d");
    PythonFactory::registerType<sofa::defaulttype::Mat3x4d>("Mat3x4d");
    PythonFactory::registerType<sofa::defaulttype::Mat4x4d>("Mat4x4d");

    PythonFactory::registerType<sofa::defaulttype::Mat2x2f>("Mat2x2f");
    PythonFactory::registerType<sofa::defaulttype::Mat3x3f>("Mat3x3f");
    PythonFactory::registerType<sofa::defaulttype::Mat3x4f>("Mat3x4f");
    PythonFactory::registerType<sofa::defaulttype::Mat4x4f>("Mat4x4f");

    // Topology
    PythonFactory::registerType<sofa::core::topology::Topology::Edge>("Edge");
    PythonFactory::registerType<sofa::core::topology::Topology::Triangle>("Triangle");
    PythonFactory::registerType<sofa::core::topology::Topology::Quad>("Quad");
    PythonFactory::registerType<sofa::core::topology::Topology::Tetra>("Tetra");
    PythonFactory::registerType<sofa::core::topology::Topology::Hexa>("Hexa");
    PythonFactory::registerType<sofa::core::topology::Topology::Penta>("Penta");

    // State vectors
    PythonFactory::registerType<sofa::defaulttype::Rigid3dTypes::VecCoord>("Rigid3d::VecCoord");
    PythonFactory::registerType<sofa::defaulttype::Rigid3fTypes::VecCoord>("Rigid3f::VecCoord");
    PythonFactory::registerType<sofa::defaulttype::Rigid3Types::VecCoord>("Rigid3::VecCoord");

    // General vectors
    for (const auto& container : containers)
    {
        // Scalars
        PythonFactory::registerType<sofa::helper::vector<std::string>>(container + "<string>");
        PythonFactory::registerType<sofa::helper::vector<float>>(container + "<float>");
        PythonFactory::registerType<sofa::helper::vector<double>>(container + "<double>");
        PythonFactory::registerType<sofa::helper::vector<bool>>(container + "<bool>");
        PythonFactory::registerType<sofa::helper::vector<int>>(container + "<int>");

        // vectors
        PythonFactory::registerType<sofa::helper::vector<sofa::defaulttype::Vec2d>>(container + "<Vec2d>");
        PythonFactory::registerType<sofa::helper::vector<sofa::defaulttype::Vec3d>>(container + "<Vec3d>");
        PythonFactory::registerType<sofa::helper::vector<sofa::defaulttype::Vec4d>>(container + "<Vec4d>");
        PythonFactory::registerType<sofa::helper::vector<sofa::defaulttype::Vec6d>>(container + "<Vec6d>");

        PythonFactory::registerType<sofa::helper::vector<sofa::defaulttype::Vec2f>>(container + "<Vec2f>");
        PythonFactory::registerType<sofa::helper::vector<sofa::defaulttype::Vec3f>>(container + "<Vec3f>");
        PythonFactory::registerType<sofa::helper::vector<sofa::defaulttype::Vec4f>>(container + "<Vec4f>");
        PythonFactory::registerType<sofa::helper::vector<sofa::defaulttype::Vec6f>>(container + "<Vec6f>");

        // Matrices
        PythonFactory::registerType<sofa::helper::vector<sofa::defaulttype::Mat2x2d>>(container + "<Mat2x2d>");
        PythonFactory::registerType<sofa::helper::vector<sofa::defaulttype::Mat3x3d>>(container + "<Mat3x3d>");
        PythonFactory::registerType<sofa::helper::vector<sofa::defaulttype::Mat3x4d>>(container + "<Mat3x4d>");
        PythonFactory::registerType<sofa::helper::vector<sofa::defaulttype::Mat4x4d>>(container + "<Mat4x4d>");

        PythonFactory::registerType<sofa::helper::vector<sofa::defaulttype::Mat2x2f>>(container + "<Mat2x2f>");
        PythonFactory::registerType<sofa::helper::vector<sofa::defaulttype::Mat3x3f>>(container + "<Mat3x3f>");
        PythonFactory::registerType<sofa::helper::vector<sofa::defaulttype::Mat3x4f>>(container + "<Mat3x4f>");
        PythonFactory::registerType<sofa::helper::vector<sofa::defaulttype::Mat4x4f>>(container + "<Mat4x4f>");


        // Topology
        PythonFactory::registerType<sofa::helper::vector<sofa::core::topology::Topology::Edge>>(container + "<Edge>");
        PythonFactory::registerType<sofa::helper::vector<sofa::core::topology::Topology::Triangle>>(container + "<Triangle>");
        PythonFactory::registerType<sofa::helper::vector<sofa::core::topology::Topology::Quad>>(container + "<Quad>");
        PythonFactory::registerType<sofa::helper::vector<sofa::core::topology::Topology::Tetra>>(container + "<Tetra>");
        PythonFactory::registerType<sofa::helper::vector<sofa::core::topology::Topology::Hexa>>(container + "<Hexa>");
        PythonFactory::registerType<sofa::helper::vector<sofa::core::topology::Topology::Penta>>(container + "<Penta>");
    }
    return true;
}

void PythonFactory::uniqueKeys(std::back_insert_iterator<sofa::helper::vector<std::string> > it)
{
    std::transform(s_dataCreationFct.begin(), s_dataCreationFct.end(),
                   it, [](const auto& item){ return item.first; });
}

} /// sofapython3




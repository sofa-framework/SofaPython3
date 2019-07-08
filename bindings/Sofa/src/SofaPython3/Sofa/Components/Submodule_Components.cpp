/*********************************************************************
Copyright 2019, Inria, CNRS, University of Lille

This file is part of runSofa2

runSofa2 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

runSofa2 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with sofaqtquick. If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/
/********************************************************************
 Contributors:
    - damien.marchal@univ-lille.fr
********************************************************************/

#include <iostream>
#include <pybind11/embed.h>

#include <sofa/core/ObjectFactory.h>
using sofa::core::objectmodel::BaseObjectDescription;
using sofa::core::ObjectFactory;

#include <sofa/helper/logging/Messaging.h>
using sofa::helper::logging::Message;

#include <SofaPython3/DataHelper.h>
#include <SofaPython3/Sofa/Core/Binding_BaseObject.h>
#include <SofaPython3/Sofa/Core/Binding_Node.h>
#include "Submodule_Components.h"

namespace py = pybind11;
using namespace py::literals;

namespace sofapython3
{

py::cpp_function f { [](const std::string& s){ std::cout << "COUCOU: " << s << std::endl;} };

class FCreator
{
public:
    FCreator(const std::string& n)
    {
        name = n;
    }
    std::string name;
};

py::module addSubmoduleComponents(py::module &m)
{
    py::class_<FCreator> mm(m, "Creator");
    mm.def("__call__", [](FCreator& s, const py::args& args, const py::kwargs& kwargs){
        if(args.size() != 1)
        {
            throw py::type_error("Invalid number of arguments. Only 1 argument of type 'Node' is allowed.");
        }

        py::object pynode = args[0];
        if(!py::isinstance<sofa::simulation::Node>(pynode))
        {
            throw py::type_error(std::string("Invalid first argument. Expecting 'Node' but got ") + py::cast<std::string>(py::str(pynode)));
        }

        auto node = py::cast<sofa::simulation::Node*>(pynode);

        /// Prepare the description to hold the different python attributes as data field's
        /// arguments then create the object.
        BaseObjectDescription desc {s.name.c_str(), s.name.c_str()};
        fillBaseObjectdescription(desc, kwargs);
        auto object = ObjectFactory::getInstance()->createObject(node, &desc);

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

        //checkParamUsage(object.get(), desc);

        /// Convert the logged messages in the object's internal logging into python exception.
        /// this is not a very fast way to do that...but well...python is slow anyway. And serious
        /// error management has a very high priority. If performance becomes an issue we will fix it
        /// when needed.
        if(object->countLoggedMessages({Message::Error}))
        {
            throw py::value_error(object->getLoggedMessagesAsString({Message::Error}));
        }

        return py::cast(object);
    });
    py::module simulation = m.def_submodule("Components");

    simulation.def("__getattr__", [](const std::string& name) -> py::object
    {
        return py::cast(new FCreator(name));
    });

    // THIS PART IS NOT WORKING...
    //todo(dmarchal 04/07/2019): do it or delete it in one year.
//    py::list names;
//    std::vector<ObjectFactory::ClassEntry::SPtr> entries ;
//    ObjectFactory::getInstance()->getAllEntries(entries) ;

//    for(auto& entry : entries)
//    {
//       names.append(entry->className);
//    }

//    simulation.def("__dir__", []() -> py::object
//    {
//                       py::list names;
//                       std::vector<ObjectFactory::ClassEntry::SPtr> entries ;
//                       ObjectFactory::getInstance()->getAllEntries(entries) ;

//                       for(auto& entry : entries)
//                       {
//                          names.append(entry->className);
//                       }
//                       return names; //std::move(names);
//                   });


    return simulation;
}

} /// namespace sofapython3

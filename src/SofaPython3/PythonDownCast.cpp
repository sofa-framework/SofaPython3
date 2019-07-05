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

#include <functional>
#include "PythonDownCast.h"
#include <SofaSimulationGraph/DAGNode.h>

namespace sofapython3
{

std::map<std::string, downCastingFunction> PythonDownCast::s_downcastingFct;

std::map<std::string, downCastingFunction>::iterator PythonDownCast::searchLowestCastAvailable(const sofa::core::objectmodel::BaseClass* metaclass)
{
    /// If there is a match with current metaclass we returns it
    auto kv = s_downcastingFct.find(metaclass->className);
    if( kv != s_downcastingFct.end())
    {
        return kv;
    }

    /// If there is no match we try to find if there is a match for a parent of the requested type
    /// As this is a slow process we cache the results to speed the subsequent request.
    for(auto p : metaclass->parents)
    {
        auto kvs = searchLowestCastAvailable(p);
        if( kvs != s_downcastingFct.end() )
        {
            return kvs;
        }
    }

    throw std::runtime_error("Unable to find a python binding for an object in-heriting from Base.");
}

py::object PythonDownCast::toPython(sofa::core::objectmodel::Base* object)
{
    auto metaclass = object->getClass();

    /// Let's first search if there is a casting function for the give type.
    auto kv = s_downcastingFct.find(metaclass->className);
    if( kv != s_downcastingFct.end())
    {
        return kv->second(object);
    }

    /// If the first search fail we will starting moving forward in the inheritance graph to
    /// detect which of its parents has a downcast function available. This function stops
    /// at first match.
    kv = searchLowestCastAvailable(metaclass);

    s_downcastingFct[metaclass->className] = kv->second;
    return kv->second(object);
}

} /// sofapython3




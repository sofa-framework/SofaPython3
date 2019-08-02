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
#pragma once

#include <sofa/helper/Factory.h>
#include <sofa/helper/Factory.inl>
#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/Event.h>
#include <pybind11/pybind11.h>
#include "config.h"

/////////////////////////////// DECLARATION //////////////////////////////
namespace sofapython3
{
    /// Makes an alias for the pybind11 namespace to increase readability.
    namespace py { using namespace pybind11; }

    typedef std::function<py::object(sofa::core::objectmodel::Base*)> componentDowncastingFunction;
    typedef std::function<py::object(sofa::core::objectmodel::BaseData*)> dataDowncastingFunction;
    typedef std::function<py::dict(sofa::core::objectmodel::Event*)> eventDowncastingFunction;
    typedef std::function<sofa::core::objectmodel::BaseData*()> dataCreatorFunction;

    typedef sofa::helper::Factory< std::string, sofa::core::objectmodel::BaseData> DataFactory;

    class SOFAPYTHON3_API PythonFactory
    {
        static std::map<std::string, componentDowncastingFunction> SOFAPYTHON3_API s_componentDowncastingFct;
        static std::map<std::string, dataDowncastingFunction> SOFAPYTHON3_API s_dataDowncastingFct;
        static std::map<std::string, eventDowncastingFunction> SOFAPYTHON3_API s_eventDowncastingFct;
        static std::map<std::string, dataCreatorFunction> SOFAPYTHON3_API s_dataCreationFct;
    public:
        static py::object SOFAPYTHON3_API toPython(sofa::core::objectmodel::Base* object);
        static py::object SOFAPYTHON3_API toPython(sofa::core::objectmodel::BaseData* data);
        static py::object SOFAPYTHON3_API valueToPython_ro(sofa::core::objectmodel::BaseData* data);
        static void SOFAPYTHON3_API fromPython(sofa::core::objectmodel::BaseData* data, const py::object& value);
        static py::object SOFAPYTHON3_API toPython(sofa::core::objectmodel::Event* event);
        static sofa::core::objectmodel::BaseData* SOFAPYTHON3_API createInstance(const std::string& typeName);

        template<class T>
        static void SOFAPYTHON3_API registerType(componentDowncastingFunction fct)
        {
            PythonFactory::s_componentDowncastingFct[T::GetClass()->className] = fct;
        }
        template<class T>
        static void SOFAPYTHON3_API registerType(const std::string& typeName, dataDowncastingFunction fct)
        {
            PythonFactory::s_dataDowncastingFct[typeName] = fct;
        }
        template<class T>
        static void SOFAPYTHON3_API registerType(eventDowncastingFunction fct)
        {
            PythonFactory::s_eventDowncastingFct[T::GetClassName()] = fct;
        }

        template <class T>
        static void SOFAPYTHON3_API registerType(const std::string& s) {
            s_dataCreationFct[s] = [](){ return new sofa::core::objectmodel::Data<T>(); };
        }

        static std::map<std::string, componentDowncastingFunction>::iterator SOFAPYTHON3_API searchLowestCastAvailable(const sofa::core::objectmodel::BaseClass* metaclass);

        static void SOFAPYTHON3_API uniqueKeys(std::back_insert_iterator<sofa::helper::vector<std::string> > it);
    private:
        static bool registerDefaultEvents();
        static bool registerDefaultTypes();
        static bool defaultEventsRegistered;
        static bool defaultTypesRegistered;
    };
} /// sofapython3





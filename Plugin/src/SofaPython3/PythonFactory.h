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
    typedef std::function<pybind11::object(sofa::core::objectmodel::Base*)> componentDowncastingFunction;
    typedef std::function<pybind11::object(sofa::core::objectmodel::BaseData*)> dataDowncastingFunction;
    typedef std::function<pybind11::dict(sofa::core::objectmodel::Event*)> eventDowncastingFunction;
    typedef std::function<sofa::core::objectmodel::BaseData*()> dataCreatorFunction;

    typedef sofa::helper::Factory< std::string, sofa::core::objectmodel::BaseData> DataFactory;

    class SOFAPYTHON3_API PythonFactory
    {
    public:
        static pybind11::object toPython(sofa::core::objectmodel::Base* object);
        static pybind11::object toPython(const sofa::core::objectmodel::BaseData* data);
        static pybind11::object toPython(sofa::core::objectmodel::BaseData* data);
        static pybind11::object valueToPython_ro(sofa::core::objectmodel::BaseData* data);
        static void fromPython(sofa::core::objectmodel::BaseData* data, const pybind11::object& value);
        static pybind11::object toPython(sofa::core::objectmodel::Event* event);
        static sofa::core::objectmodel::BaseData* createInstance(const std::string& typeName);

        static void registerType(const std::string& typeName, componentDowncastingFunction fct);
        static void registerType(const std::string& typeName, dataDowncastingFunction fct);
        static void registerType(const std::string& typeName, eventDowncastingFunction fct);
        static void registerType(const std::string& typeName, dataCreatorFunction fct);

        template<class T>
        static void registerType(componentDowncastingFunction fct)
        {
            registerType(T::GetClass()->typeName, fct);
        }

        template<class T>
        static void registerType(eventDowncastingFunction fct)
        {
            registerType(T::GetClassName(), fct);
        }

        template <class T>
        static void registerType(const std::string& s) {
			sofa::core::objectmodel::Data<T> a; //Solve compilation for Windows, apparently force instanciation for Data<T> ???
            registerType(s, [](){ return new sofa::core::objectmodel::Data<T>(); });
        }

        static std::map<std::string, componentDowncastingFunction>::iterator searchLowestCastAvailable(const sofa::core::objectmodel::BaseClass* metaclass);

        static void uniqueKeys(std::back_insert_iterator<sofa::helper::vector<std::string> > it);
    private:
        static bool registerDefaultEvents();
        static bool registerDefaultTypes();
        static bool defaultEventsRegistered;
        static bool defaultTypesRegistered;
    };
} /// sofapython3





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

#include <sofa/core/objectmodel/Base.h>
#include <pybind11/pybind11.h>

/////////////////////////////// DECLARATION //////////////////////////////
namespace sofapython3
{
    /// Makes an alias for the pybind11 namespace to increase readability.
    namespace py { using namespace pybind11; }

    typedef std::function<py::object(sofa::core::objectmodel::Base*)> downCastingFunction;

    class PythonDownCast
    {
    public:
        static py::object toPython(sofa::core::objectmodel::Base* object);

        template<class T>
        static void registerType(downCastingFunction fct)
        {
            s_downcastingFct[T::GetClass()->className] = fct;
        }

        static std::map<std::string, downCastingFunction>::iterator searchLowestCastAvailable(const sofa::core::objectmodel::BaseClass* metaclass);

    private:
        static std::map<std::string, downCastingFunction> s_downcastingFct;
    };
} /// sofapython3





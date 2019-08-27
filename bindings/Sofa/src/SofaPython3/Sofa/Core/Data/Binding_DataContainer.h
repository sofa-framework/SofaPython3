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

#include <pybind11/pybind11.h>
#include <sofa/core/objectmodel/BaseData.h>

namespace sofapython3
{
/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }
using sofa::core::objectmodel::BaseData;

void moduleAddDataContainer(py::module& m);
void moduleAddDataContainerContext(py::module& m);


class DataContainer : public BaseData {
public:
    py::tuple getShape();
    size_t getNDim();
    size_t getSize();
} ;

/// @brief A python context manager to control writeable state of a DataContainer.
///
/// This context manager handle in a graceful way the writeable state of a DataContainer
/// so the Data counter gets updated correctly when we left the context.
///
/// It allows to implement stuff like that:
///   with myobject.position.arrayContext() as wa:
///         wa[3] = [1,2,3]
///         wa[4] = ...
///  # When we leave the context the data counter gets updated
///
/// NB: context is a classic python idom defined in
///    https://docs.python.org/3/reference/datamodel.html#context-managers
///
class DataContainerContext
{
public:
    DataContainerContext(BaseData* data_, py::object fct_) : data(data_), fct(fct_){}

    BaseData* data {nullptr};
    py::object wrap;
    py::object fct;
};

}

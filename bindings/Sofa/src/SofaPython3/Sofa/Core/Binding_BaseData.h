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

#include <sofa/core/objectmodel/BaseData.h>
#include <SofaPython3/DataHelper.h>

/// More info about smart pointer in
/// pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html
/// BaseData are raw ptr so we use the raw_ptr smart pointer.
/// If you have a better way to do that, please make a PR.
PYBIND11_DECLARE_HOLDER_TYPE(BaseData, sofapython3::raw_ptr<BaseData>)

template class pybind11::class_<sofa::core::objectmodel::BaseData, sofapython3::raw_ptr<sofa::core::objectmodel::BaseData>>;


namespace sofapython3
{
    void moduleAddBaseData(py::module& m);
} /// sofapython3

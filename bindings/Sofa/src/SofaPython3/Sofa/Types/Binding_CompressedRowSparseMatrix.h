/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2021 INRIA, USTL, UJF, CNRS, MGH                     *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#pragma once

#include <pybind11/pybind11.h>

namespace sofapython3 {

<<<<<<<< HEAD:bindings/Sofa/src/SofaPython3/Sofa/Types/Binding_CompressedRowSparseMatrix.h
void moduleAddCompressedRowSparseMatrix(pybind11::module& m);

}  // namespace sofapython3
========
/// Forward declaration in pybind11.
/// more details in: https://github.com/sofa-framework/SofaPython3/pull/457
void moduleForwardAddBaseClass(pybind11::module& m);
void moduleAddBaseClass(pybind11::module& m);

} /// namespace sofapython3
>>>>>>>> xp-component-stubgen:bindings/Sofa/src/SofaPython3/Sofa/Core/Binding_BaseClass.h

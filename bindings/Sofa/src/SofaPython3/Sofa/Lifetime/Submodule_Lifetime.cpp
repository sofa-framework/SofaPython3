/******************************************************************************
*                              SofaPython3 plugin                             *
*                  (c) 2021 CNRS, University of Lille, INRIA                  *
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

#include <iostream>
#include <map>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <SofaPython3/lifetime/features.h>

namespace py { using namespace pybind11; }

namespace sofapython3
{

/// The first parameter must be named the same as the module file to load.
PYBIND11_MODULE(Lifetime, ffmodule)
{
    ffmodule.doc() = R"doc(
           Control the the activation of new features
           ------------------------------------------
           Sofa.Lifetime.object_auto_init = True
       )doc";
    ffmodule.def("init", sofapython3::lifetime::features::init);
    ffmodule.def("set", sofapython3::lifetime::features::set);
    ffmodule.def("get", sofapython3::lifetime::features::get);
    ffmodule.def("list_features", sofapython3::lifetime::features::list_features);
}

} ///namespace sofapython3

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


#include <pybind11/pybind11.h>
#include <SofaPython3/SofaTypes/Binding_Mat.h>
#include <SofaPython3/SofaTypes/Binding_Quat.h>
#include <SofaPython3/SofaTypes/Binding_Vec.h>
#include <sofa/defaulttype/init.h>

/// The first parameter must be named the same as the module file to load.
PYBIND11_MODULE(SofaTypes, m) {
    sofa::defaulttype::init();

    moduleAddMat(m);
    moduleAddQuat(m);
    moduleAddVec(m);
}

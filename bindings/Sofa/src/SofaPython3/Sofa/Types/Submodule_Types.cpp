/******************************************************************************
*                              SofaPython3 plugin                             *
*                  (c) 2019 CNRS, University of Lille, INRIA                  *
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
* Authors: see Authors.txt                                                    *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
/********************************************************************
 Contributors:
    - damien.marchal@univ-lille.fr
    - bruno.josue.marques@inria.fr
    - eve.le-guillou@centrale.centralelille.fr
    - jean-nicolas.brunet@inria.fr
    - thierry.gaugry@inria.fr
********************************************************************/

#include <sofa/core/init.h>
#include <sofa/defaulttype/init.h>

#include <SofaPython3/Sofa/Types/Binding_BoundingBox.h>

namespace sofapython3 {
/// The first parameter must be named the same as the module file to load.
PYBIND11_MODULE(Types, types)
{
    // These are needed to force the dynamic loading of module dependencies (found in CMakeLists.txt)
    sofa::core::init();
    sofa::defaulttype::init();

    types.doc() = R"doc(
           Default data types
           -------------------------------

           .. autosummary::
               Sofa.Types.BoundingBox
               ...

       )doc";

    moduleAddBoundingBox(types);
}

}  // namespace sofapython3

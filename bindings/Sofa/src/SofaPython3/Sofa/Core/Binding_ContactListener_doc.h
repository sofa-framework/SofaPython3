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

#pragma once

namespace sofapython3::doc::contactListener {

static auto contactListenerClass =
        R"(
        ContactListener is an API dedicated to the monitoring of two collision models.

        :Example of use:

        .. code-block:: python

            import Sofa.Core
            import SofaRuntime
            SofaRuntime.importPlugin("SofaComponentAll")

            root = Sofa.Core.Node("root")
            model1 = root.addObject("TriangleCollisionModel")
            model2 = root.addObject("PointCollisionModel")
            listener = root.addObject(
                "ContactListener",
                collisionModel1=model1.getLinkPath(),
                collisionModel2=model2.getLinkPath(),
            )
            print(listener.getNumberOfContacts())
            print(listener.getDistances())
        )";

} // namespace sofapython3::doc::contactListener

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

namespace sofapython3::doc::baseCamera {

static auto baseCameraClass =
        R"(
        API dedicated to the control of the Camera in SOFA

        Example:
        .. code-block:: python

            import Sofa.Core
            import SofaRuntime
            SofaRuntime.importPlugin("SofaComponentAll")

            root = Sofa.Core.Node("root")
            root.addObject("Camera", name="c")
            root.c.position.value # Access the position of the camera
            root.c.orientation.value # Access the orientation of the camera in quaternion

        :More complex example of use:

        The camera, added to a controller, can allow the user to create its own script to move and orientate the camera.

        In the example below, the mouse is used to orientate the camera, and the following commands are used to move it : ctrl+z to move forward,
        ctrl+x to move backward, ctrl+d to translate on the right, ctrl+q to translate on the left.
        The code for this example is available in the folder examples under the name BaseCamera.py of the SofaPython3 GitHub repository.

        .. image:: ../../../../../../docs/sphinx/source/images/baseCamera.gif
            :alt: Example of a controller for a BaseCamera
            :align: center
            :height: 250pt

        )";

} // namespace sofapython3::doc::baseCamera

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

namespace sofapython3::doc::visualparams {

static auto baseVisualParamsClass =
        R"(
        VisualParams is a component that manages and provides rendering-related parameters for the simulation's objected.
        It determine draw distance, access the camera position or viewing frustum and handle display flags
        like wireframe, texture, lighting as well as provide a drawTool object that can be used to render things.

        Example:
            class MyController(Sofa.Core.Controller):
                def __init__(self, *args, *kwargs):
                    Sofa.Core.Controller.__init__(self, *args, **kwargs)

                def draw(self, visual_params):
                    dt = visual_param.getDrawTool()
                    dt.drawPoint([0,0,0])
        )";

} // namespace sofapython3::doc::visualParams

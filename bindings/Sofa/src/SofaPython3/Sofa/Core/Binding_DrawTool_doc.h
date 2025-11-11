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

namespace sofapython3::doc::drawtool
{

static auto baseDrawToolClass =
        R"(DrawTool is a wrapper for low-level rendering draw calls.
           It provides higher-level drawing functions like drawing lines, points, spheres, arrows, etc., without
           needing to write raw OpenGL each time.)";

static auto drawPoints =
            R"(Draws 3D points.
            points: NumPy array of shape (N, 3) or BaseData of type vector<Vec3>
            size: Point size (float)
            color: RGBAColor object)";

static auto drawLines =
            R"(Draws connected line segments between given 3D points.)";


static auto drawTriangles =
            R"(Draws a triangle mesh from a flat array of 3-point group.
            points: NumPy array of shape (N, 3) or BaseData of type vector<Vec3>
            color: RGBAColor object)";

static auto drawQuads =
            R"(Draws quad mesh from a flat array of 4-point groups.
            points: NumPy array of shape (N, 3) or BaseData of type vector<Vec4>
            color: RGBAColor object)";

static auto drawSpheres =
            R"(Draws spheres from a set of points.
            points: NumPy array of shape (N, 3) or BaseData of type vector<Vec4>
            color: RGBAColor object)";

static auto drawBoundingBox =
            R"(Draws a bounding box from its min and max point
            min: an array of 3 values
            max: an array of 3 values)";

static auto drawFrames =
            R"(Draws 3d frames from position and orientation
            position: an array of size N,3
            orientaiton: an array of size N,4)";

static auto drawText =
            R"(Draws text in 3D
            position: an array of size 3
            size: dimmension of the text,
            text: content to show on screen
            color: RGBAColor)";

static auto drawOverlayText =
            R"(Draws text in 2D
            position: an array of size 2
            size: the font size,
            text: content to show on screen
            color: RGBAColor)";


}

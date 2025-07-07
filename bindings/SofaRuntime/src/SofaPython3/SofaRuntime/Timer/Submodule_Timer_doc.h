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

namespace sofapython3::doc::Timer
{
static auto clear =
R"(
Clears the AdvancedTimer.
)";

static auto isEnabled =
R"(
Returns True if the given id is enabled, False otherwise.
)";

static auto setEnabled =
R"(
Enables or disables the given timer.
)";

static auto getInterval =
R"(
Returns the Timer's interval.
)";

static auto setInterval =
R"(
Sets the interval for the given timer.
)";

static auto setOutputType =
R"(
Set the outputType for the given AdvancedTimer.

@param id string id of the timer
@param type string output type name (example : "json")
)";
}
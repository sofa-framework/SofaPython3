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

#include <sofa/config.h>
#include <string>
#include <vector>

namespace sofapython3::lifecycle::features
{

/// Retrieve the value associated with the feature "name"
/// raise an exception if "name" is not existing.
SOFA_EXPORT_DYNAMIC_LIBRARY bool get(const std::string& name);

/// Change the value associated with the feature "name"
/// raise an exception if "name" is not existing.
SOFA_EXPORT_DYNAMIC_LIBRARY void set(const std::string& name, bool value);

/// Create and set a new value for feature with "name"
SOFA_EXPORT_DYNAMIC_LIBRARY void init(const std::string& name, bool value);

/// Returns the list of registered features names
SOFA_EXPORT_DYNAMIC_LIBRARY std::vector<std::string> list_features();

} ///namespace  sofapython3::futurefeatures

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

namespace sofapython3::doc::objectmodel {
static auto ObjectFactoryClass =
    R"(
    Main class used to register and dynamically create objects.

    It uses the Factory design pattern, where each class is registered in a map,
    and dynamically retrieved given the type name.

    It also stores metainformation on each classes, such as description,
    authors, license, and available template types.
    )";

static auto ClassEntryClass =
    R"(Record storing information about a class)";

static auto ObjectFactory_getEntry =
    R"(
    Get an entry given a class name (or alias)
    )";

static auto ObjectFactory_shortName =
    R"(
    Return the shortname for this classname. Empty string if no creator exists for this classname.
    )";

static auto ObjectFactory_getAllEntries =
    R"(
    Return all the registered classes
    )";

static auto ObjectFactory_getEntriesFromTarget =
    R"(
    Return the registered classes from a given target
    )";

static auto ObjectFactory_targets =
    R"(
    Targets which contains a set of components. For example, the target 'SofaBaseMechanics'.
    )";

static auto ObjectFactory_getInstance =
    R"(
    Get the ObjectFactory singleton instance
    )";
} /// namespace sofapython3::doc::forceField

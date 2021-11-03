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

namespace sofapython3::doc::base
{
static auto BaseClass =
        R"(
        Sofa.Core.Base is the root of the Sofa class Hierarhcy
        All objects are in-heriting from this one.
        )";

static auto getName =
        R"(
        Return the name of the entity
        :rtype: string
        )";
static auto setName =
        R"(
        Set the name of this object
        :param n
        :type n: string
        )";

static auto setNameCounter =
        R"(
        Set the name of this object, adding an integer counter
        :param n
        :param counter
        :type n: string
        :type counter: integer
        )";

static auto getClass =
        R"(
        Return the class of the object
        )";

static auto findData =
        R"(
        Find a data field given its name.
        Return NULL if not found.
        If more than one field is found (due to aliases), only the first is returned.
        :param name
        :type name: string
        :return: the data field
        )";
static auto getLinkPath =
        R"(
        returns the path name as a parsable string.
        eg:
            if object.getPathName() is "/child1/object"
            then the linkPath is @"/child1/object"
        )";
static auto getPathName =
        R"(
        returns the path name as a string.
        .. code-block:: python
            a = Sofa.Core.Node("root")
            b.addObject("Camera", name="camera")
            b.getPathName() # should returns "/root/camera"
        ```
        )";
static auto setDataValues =
        R"(
        Set values for a the given data field, multiple pairs of args are allowed.
        )";
static auto getDataFields =
        R"(
        Accessor to the vector containing all the fields of this object
        :return: A vector containing the data fields
        )";

static auto getLoggedMessagesAsString =
        R"(
        Returns a single string with all the messages logged in the internal buffer of a sofa object.
        Return empty string if there is no messages.
        )";

static auto countLoggedMessages =
        R"(
        Returns the number of messages in the object's logs.
        )";

static auto clearLoggedMessages =
        R"(
        Remove all logged messages in the object's logs.
        )";


static auto findLink =
        R"(
        Find a link given its name.
        Return NULL if not found.
        If more than one link is found (due to aliases), only the first is returned.
        :param name: the name of the link
        :type name: string
        :return: the link
        )";

static auto getLinks =
        R"(
        Accessor to the vector containing all the links of this object
        :return: A vector containing the links
        )";

static auto addData =
        R"(
        Create a data field, then adds it to the base.
        Note that this method should only be called if the field was not initialized with the initData method
        :param self: the base itself
        :param name: the name of the data to be added
        :param value: the value from which the data can be created
        :param help: help message that describes the data to be created
        :param group: the group the data belongs to
        :param type: the type of the data
        :type self: object
        :type name: string
        :type value: object
        :type help: string
        :type group: string
        :type type: string
        )";

static auto addLink =
        R"(
        Create a Link to a SOFA component and adds it to the base.
        Note that this method should only be called if the field was not initialized with the initLink method
        :param self: the base itself
        :param name: the name of the link to be added
        :param value: the value from which the data can be created (either a pathname or a SofaBase)
        :param help: help message that describes the link to be created
        :type self: object
        :type name: string
        :type value: object
        :type help: string
        )";

static auto addDataInitialized =
        R"(
        Add a data field.
        Note that this method should only be called if the field was not initialized with the initData method
        :param self: the base itself
        :param d: the data to be added
        :type self: Base*
        :type d: object
        )";


static auto getData =
        R"(
        Get the data field given its name.
        :param self:
        :param s:
        :type self: Base&
        :type s: string
        :return: the first data found of this name
        )";

static auto getClassName =
        R"(
        Get the name of the class of the Base.
        )";
static auto getTemplateName =
        R"(
        Get the name of the template of the Base.
        )";
  
static auto getDefinitionSourceFilePos="Returns the line number where the object is defined.";
static auto getDefinitionSourceFileName="Returns the name of the file that contains the object definition.";
static auto getInstanciationSourceFilePos="Returns the line number where the object is instanciatiated.";
static auto getInstanciationSourceFileName="Returns the name of the file where this object instance is from.";

static auto setDefinitionSourceFilePos="Set the line number where the object is defined.";
static auto setDefinitionSourceFileName="set the name of the file that contains the object definition.";
static auto setInstanciationSourceFilePos="Set the line number where the object is instanciatiated.";
static auto setInstanciationSourceFileName="Set the name of the file where this object instance is from.";

}

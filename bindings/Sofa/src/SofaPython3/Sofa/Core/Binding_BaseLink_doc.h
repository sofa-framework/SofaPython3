#pragma once

namespace sofapython3::doc::baseLink
{
static auto baseLinkClass =
        R"(
        A Link allows you to access a sofa component from another one anywhere in the simulation graph.
        In your scene creation file, it usually appear as : input=@../component.

        :Example of use:

        .. code-block:: python

            import Sofa.Core
            import SofaRuntime
            SofaRuntime.importPlugin("SofaAllCommonComponents")

            root = Sofa.Core.Node("root")
            root.addObject("MechanicalObject", name="t")
            link = root.findLink("mechanicalState") # access the link
            link.getValueString() # will print '@/t'
            link.getHelp() # will print the help message for all mechanical states
        )";

static auto getName =
        R"(
        Get the name of the link.
        :rtype: string
        )";

static auto setName =
        R"(
        Set a new name for the link.
        :param name: the name to be given to the link
        :type name: string
        )";
static auto getHelp =
        R"(
        Get the help message.
        :rtype: string
        )";
static auto setHelp =
        R"(
        Set the help message.
        :param message: the new help message
        :type message: string
        )";
static auto getOwnerBase =
        R"(
        Get the base owner of the link.
        :rtype: Base
        )";
static auto getOwnerData =
        R"(
        Get the data of the owner of the link.
        :rtype: Data
        )";
static auto isMultiLink =
        R"(
        Checks whether the link is an array.
        :return: true if link is an array.
        :rtype: boolean
        )";
static auto setPersistent =
        R"(
        Alias to match BaseData API.
        Changes the value of a flag,
        that should be set to true if link requires a path string in order to be created.
        :param bool: new value for the flag.
        :type bool: boolean
        )";
static auto isPersistent =
        R"(
        Alias to match BaseData API.
        Checks whether the link requires a path in order to be created.
        :return: true if the link requires a path.
        :rtype: boolean
        )";
static auto isReadOnly =
        R"(
        Alias to match BaseData API.
        Checks whether the link requires a path in order to be created.
        :return: true if the link doesn't require a path.
        :rtype: boolean
        )";
static auto getSize =
        R"(
        Get the size of the link.
        :rtype: size_t
        )";
static auto getLinkedBase =
        R"(
        Get the linked base.
        :param index: index of the linked base.
        :type index: int
        )";
static auto getLinkedData =
        R"(
        Get the linked Data.
        :param index: index of the linked Data.
        :type index: int
        )";
static auto getLinkedPath =
        R"(
        Get the linked path.
        :param index: index of the linked path.
        :type index: int
        )";
static auto read =
        R"(
        Read the command line.
        :param str: the content of the command line
        :type str: string
        )";
static auto getValueString =
        R"(
        Print the value of the associated variable.
        )";
static auto getValueTypeString =
        R"(
        Print the value of the associated variable.
        )";
}

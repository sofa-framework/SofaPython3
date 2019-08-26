#pragma once

namespace sofapython3::doc::baseData
{
static auto BaseDataClass =
        R"(
        The purpose of a data is to store the attributs of your component.

        :Create a data field:

        To create a data field, you can use the function addData, as shown in the example below. Note that the help
        parameter allows you  to create a help message for the data field, accessible with the getHelp method.

        .. code-block:: python

            import Sofa.Core

            root = Sofa.Core.Node("root")
            root.addData(name="aField", value=1.0 , help="help message",group="theDataGroup",
                type="float")

        :Access the value of a data field in read-only:

        Accessing a data field is easy thanks to the generalized access API. You simply need the name of the data field,
        as shown in the following :

        .. code-block:: python

            import Sofa.Core

            root = Sofa.Core.Node("root")
            root.addData(name="aField", value=1.0 , help="help message",group="theDataGroup",
                type="float")
            valueOfaField = root.aField.value
            print(valueOfaField) #will print '1.0'

        Please note that here valueOfaField is a copy of the real value of aField. You can change its value, but it won't change the value of aField.

        SOFA elements are often created with data fields, so you won't always need to create data fields.
        For example, most SOFA elements are created with the data field `name` :

        .. code-block:: python

            import Sofa.Core

            root = Sofa.Core.Node("rootNode")
            print(root.name.value) #will print 'rootNode'

        However, this API only allows you to access the value of a data field, but not to change it.

        :Change the value of a data field:

        To change the value of a data field, there are two possibilities. If the type of the data field is a scalar, you can do the following :

        .. code-block:: python

            import Sofa.Core

            root = Sofa.Core.Node("root")
            root.addData(name="aField", value=1.0 , help="help message",group="theDataGroup",
                type="float")
            root.aField = 3.0 # will change the value of aField
            print(root.aField.value) # will print '3.0'

        Or you can create a temporary pointer that will allow you to change the value of aField, instead of giving it a whole new one,
        which is more cost effective when the type of the data field is something other than a scalar:

        .. code-block:: python

            import Sofa.Core

            root = Sofa.Core.Node("root")
            root.addData(name="aField", value=[0.0, 1.0, 3.0] , help="help message",group="theDataGroup",
                type="Vec3d")
            with root.aField.writeable() as value:
                    value[0] = 3.0

            print(root.aField.value) # will print '[3.0, 1.0, 3.0]'

        )";
static auto getName =
        R"(
        Get the name of the data.
        :rtype: string
        )";
static auto getCounter =
        R"(
        Return the number of changes since creation.
        This can be used to efficiently detect changes.
        :rtype: integer
        )";
static auto getHelp =
        R"(
        Get the help message.
        :rtype: string
        )";
static auto unset =
        R"(
        Reset the isSet flag to false, to indicate that the current value is the default for this data.
        )";
static auto getOwner =
        R"(
        Return the Base component owning this data.
        :rtype: Base
        )";
static auto getParent =
        R"(
        Return the parent data of this data.
        :rtype: BaseData
        )";
static auto typeName =
        R"(
        Return the name of the type of the data.
        :rtype: string
        )";
static auto getPathName =
        R"(
        Return the name of the path of the data.
        :rtype: string
        )";
static auto getLinkPath =
        R"(
        Return the path of the link to the data.
        :rtype: string
        )";
static auto hasChanged =
        R"(
        Update the data if it is dirty.
        :return: true if it is updated, false otherwise.
        ;rtype: boolean
        )";
static auto isSet =
        R"(
        Check if the value has been modified
        If this data is linked, the value of this data will be considered as modified
        (even if the parent's value has not been modified)
        :return: True if the data has been modified
        )";
static auto toList =
        R"(
        Return a list containing the value of the vector of data.
        )";
static auto array =
        R"(
        Return a python read-only array of the data.
        :rtype: read-only array
        )";
static auto writeableArrayArg =
        R"(
        Return a writable array of the data given an Object.
        :param f: the object to convert into an array.
        :type f: python object
        :rtype: writeable array
        )";
static auto writeableArray =
        R"(
        Return a new empty array.
        :rtype: writeable array
        )";
static auto setName =
        R"(
        Set a new name for the data.
        :param name: the new name
        :type name: string
        )";
static auto getValueString =
        R"(
        Print the value of the associated variable.
        )";
static auto getValueTypeString =
        R"(
        Print the value of the associated variable.
        )";
static auto read =
        R"(
        Read the command line, set the value of the data to the given content of the command line.
        :param str: the content of the command line
        :type str: string
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
static auto setParent =
        R"(
        Set a new parent and a new path for the data.
        :param parent: the new parent of the data
        :param path: the new path of the data
        :type parent: BaseData
        :type path: string
        )";
static auto hasParent =
        R"(
        Check whether the data has a parent.
        :return: true if the data has a parent.
        :rtype: boolean
        )";
static auto getAsACreateObjectParameter =
        R"(
        Return the full path name of this baseObject with an extra prefix '@'
        :rtype: string
        )";
static auto isDirty =
        R"(
        Return true if the DDGNode needs to be updated.
        )";
static auto updateIfDirty =
        R"(
        Utility method to call update if necessary.
        This method should be called before reading or writing the value of this node.
        )";
static auto isReadOnly =
        R"(
        Return whether this data will be read-only in GUIs.
        :return: True for read-only
        )";
static auto setReadOnly =
        R"(
        Set whether this data is read-only. (True for read-only)
        :param bool: value of the read-only property
        :type bool: boolean
        )";
static auto isRequired =
        R"(
        Check whether a data is flagged as required for the creation of an object.
        :return: true if it is required.
        )";
static auto getValueVoidPtr =
        R"(
        Get a constant void pointer to the value held in this data.
        )";
}

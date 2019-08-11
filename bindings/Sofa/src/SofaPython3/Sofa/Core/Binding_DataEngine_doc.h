#pragma once

namespace sofapython3::doc::dataengine
{
static auto DataEngine =
        R"(
        A trampoline class that enables python implementations
        of the C++ sofa::core::DataEngine class
        )";

static auto addInput =
        R"(
        Registers a data as an input for the DataEngine.

        :param data: The data to register as input.
        :type data: Sofa.Core.Data.
        :returns:  None.
        )";
//                .. function:: addInput(data: Data)
//        :param data: the data to register as an input
//        :rtype: None
//        registers a data field as being an input for this engine.
//        Changing an input's value flags the engine as dirty,
//        which means that any access to its outputs will trigger a call to update()

static auto addOutput =
        R"(
        .. function:: addOutput(data: Sofa.Core.Data)
        registers a data field as being an output for this engine.
        Iff an Engine's output is accessed (getValue, beginEdit...)
        AND one or more of its input is dirty, the engine's update()
        method will be called
        :param data: the data to register as an output
        :rtype: None
        )";

}  // namespace sofapython3::doc::dataengine

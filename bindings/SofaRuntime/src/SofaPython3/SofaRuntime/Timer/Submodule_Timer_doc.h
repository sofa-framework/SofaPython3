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
Changes the output type for a given timer.
)";
}
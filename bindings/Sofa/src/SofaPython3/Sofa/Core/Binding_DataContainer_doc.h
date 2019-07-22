#pragma once

namespace sofapython3::doc::datacontainer
{
static auto __len__ =
        R"(
        returns the number of elements in the container's 1st
        dimension. (equivalent to shape(0))
        )";

static auto size =
        R"(
        returns the total number of elements in the container
        ( equivalent to math.prod(self.shape()) )
        )";

static auto ndim =
        R"(
        returns the number of dimensions in the container
        )";

static auto shape =
        R"(
        returns the shape of the container (a tuple of length ndim,
        with each value being equal to the length of the given dimension)
        )";

}  // namespace sofapython3::doc::datacontainer

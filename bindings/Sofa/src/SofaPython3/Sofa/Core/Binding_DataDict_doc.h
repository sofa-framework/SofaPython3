#pragma once

namespace sofapython3::doc::dataDict {

static auto Class =
        R"(
        DataDict exposes the data of a sofa object in a way similar to a normal python dictionnary.
        :Example:
        for k,v in anObject.__data__.items():
        print("Data name :"+k+" value:" +str(v)))
        )";

static auto keys =
        R"(
        Expose the data, but only the key (the name) of each items
        )";
static auto values =
        R"(
        Expose the data, but only the value of each items
        )";
static auto items =
        R"(
        Expose the data, both the key and the value of each item.
        :Example:
        for k,v in anObject.__data__.items():
        print("Data name :"+k+" value:" +str(v)))
        )";
}

/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2006 INRIA, USTL, UJF, CNRS, MGH                     *
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
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include <SofaPython3/SofaRuntime/PythonMessageHandler.h>
#include <pybind11/pybind11.h>
#include <sofa/helper/logging/Message.h>

namespace sofapython3
{

namespace
{

using namespace std::string_literals;
const std::string red { "\033[31m"s };
const std::string green { "\033[32m"s };
const std::string orange { "\033[38;5;214m"s };
const std::string magenta { "\033[35m"s };
const std::string blue { "\033[34m"s };
const std::string reset { "\033[0m"s };

std::string format(const std::string& type, const std::string& color)
{
    return color + "[" + type + "] " + reset;
};

const std::string& getPrefixText(sofa::helper::logging::Message::Type type)
{
    static const std::string advice = format("SUGGESTION", green);
    static const std::string deprecated = format("DEPRECATED", orange);
    static const std::string warning = format("WARNING", orange);
    static const std::string info = format("INFO", green);
    static const std::string error = format("ERROR", red);
    static const std::string fatal = format("FATAL", magenta);
    static const std::string empty = format("EMPTY", reset);
    static const std::string nothing{};

    switch (type)
    {
    case sofa::helper::logging::Message::Advice     : return advice;
    case sofa::helper::logging::Message::Deprecated : return deprecated;
    case sofa::helper::logging::Message::Warning    : return warning;
    case sofa::helper::logging::Message::Info       : return info;
    case sofa::helper::logging::Message::Error      : return error;
    case sofa::helper::logging::Message::Fatal      : return fatal;
    case sofa::helper::logging::Message::TEmpty     : return empty;

    case sofa::helper::logging::Message::TypeCount:
        return nothing;
    }
}

}

namespace py { using namespace pybind11; }

void PythonMessageHandler::process(sofa::helper::logging::Message &m)
{
    if (!m.sender().empty())
    {
        py::print(getPrefixText(m.type()), format(m.sender(), blue), m.messageAsString());
    }
    else
    {
        py::print(getPrefixText(m.type()), m.messageAsString());
    }
}

PythonMessageHandler& MainPythonMessageHandler::getInstance()
{
    static PythonMessageHandler s_instance;
    return s_instance;
}

} // namespace sofapython3


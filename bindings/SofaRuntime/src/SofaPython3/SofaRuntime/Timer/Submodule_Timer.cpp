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

#include "Submodule_Timer.h"
#include "Submodule_Timer_doc.h"

#include <sofa/helper/logging/Messaging.h>
#include <sofa/helper/AdvancedTimer.h>
using sofa::helper::AdvancedTimer;

#include <stack>

#include <sofa/helper/logging/Messaging.h>

namespace sofapython3
{

/**
/**
 * @brief Converts AdvancedTimer records to a Python dictionary structure
 *
 * This function processes the timer records and builds a hierarchical Python dictionary
 * that represents the timer data in a format that's easy to use in Python.
 *
 * @param id The timer ID to get records for
 * @return A Python dictionary representing the timer records
 */
py::dict getRecords(const std::string & id) {
    using sofa::helper::Record;
    using sofa::helper::system::thread::ctime_t;

    auto getTime = [](ctime_t t, ctime_t referenceTime)
    {
        constexpr double nbMillisecPerSec = 1000.;
        return nbMillisecPerSec * sofa::helper::system::thread::CTime::toSecond(t - referenceTime);
    };

    const auto records = AdvancedTimer::getRecords(id);

    // Stack of dictionaries that represents the hierarchical structure of timer records
    // Each element in the stack corresponds to a different level in the timer hierarchy.
    std::stack<py::dict> hierarchyStack;

    // Current dictionary being processed at the top of the stack
    // Represents the most recently created level in the timer hierarchy.
    py::dict currentLevel;

    py::dict token_temp;
    hierarchyStack.push(currentLevel);
    std::optional<ctime_t> referenceTime;

    for (const auto& r : records)
    {
        switch (r.type)
        {
        case Record::RNONE:
            break;
        case Record::RBEGIN: // Timer begins
        case Record::RSTEP_BEGIN: // Step begins
            currentLevel = hierarchyStack.top();
            if (currentLevel.contains(r.label.c_str()))
            {
                if (py::list::check_(currentLevel[r.label.c_str()]))
                {
                    token_temp = py::dict();
                    py::list(currentLevel[r.label.c_str()]).append(token_temp);
                    currentLevel = token_temp;
                }
                else if (py::dict::check_(currentLevel[r.label.c_str()]))
                {
                    token_temp = currentLevel[r.label.c_str()];
                    currentLevel[r.label.c_str()] = py::list();
                    py::list(currentLevel[r.label.c_str()]).append(token_temp);
                    token_temp = py::dict();
                    py::list(currentLevel[r.label.c_str()]).append(token_temp);
                    currentLevel = token_temp;
                }
                else
                {
                    msg_error("Timer::getRecords") << "Got an unexpected token of type '" << std::string(py::str(currentLevel.get_type())) << "'.";
                    break;
                }
            }
            else
            {
                // Creating a new level in the hierarchy for the current timer label
                currentLevel[r.label.c_str()] = py::dict();
                // Update the current level to the one just added
                currentLevel = currentLevel[r.label.c_str()];
            }
            if (r.type == Record::RBEGIN)
            {
                referenceTime = r.time;
            }
            if (!referenceTime.has_value())
            {
                msg_error("Timer::getRecords") << "Reference time not set.";
                break;
            }
            currentLevel["start_time"] = getTime(r.time, *referenceTime);
            hierarchyStack.push(currentLevel);
            break;
        case Record::REND: // Timer ends
        case Record::RSTEP_END: // Step ends
            currentLevel = hierarchyStack.top();
            currentLevel["end_time"] = getTime(r.time, *referenceTime);
            currentLevel["total_time"] = getTime(r.time, *referenceTime) - py::cast<float>(currentLevel["start_time"]);
            hierarchyStack.pop();
            break;
        case Record::RVAL_SET: // Sets a value
        case Record::RVAL_ADD: // Sets a value
            currentLevel = hierarchyStack.top();
            currentLevel[r.label.c_str()] = r.val;
            break;
        default:
            currentLevel = hierarchyStack.top();
            currentLevel[r.label.c_str()] = py::list();
            currentLevel = currentLevel[r.label.c_str()];
            currentLevel["start_time"] = r.time;
            break;
        }
    }

    // There should be two remaining records: Top level "record" + "timer starts". The "timer starts" record remains in
    // the stack since we normally get the records before the timer ends (ending the timer in Sofa destroys the
    // records...)
    if (hierarchyStack.size() == 2)
    {
        currentLevel = hierarchyStack.top();
        hierarchyStack.pop();
    }
    else if (hierarchyStack.size() == 1)
    {
        // This should not happen unless we successfully got the timer records AFTER the timer has ends, which would mean
        // that Sofa's advanced timer has improved, let not warn the user for that.
        currentLevel = hierarchyStack.top();
    }

    // Pop the last token ("records")
    hierarchyStack.pop();

    // The stack should be empty by now
    if (!hierarchyStack.empty())
    {
        msg_error("Timer::getRecords") << "Records stack leaked (" << hierarchyStack.size() << " elements).";
    }

    return currentLevel;
}

py::module addSubmoduleTimer(py::module &m)
{

    py::module timer = m.def_submodule("Timer");

    /// TODO: fill the docstring!
    timer.doc() = R"doc(
           Timer
           -----------------------

           Advanced timer, meant to gather precise statistics for results in published papers.
           Not so advanced for now, but it will be...
       )doc";

    timer.def("clear", AdvancedTimer::clear, doc::Timer::clear);
    timer.def("isEnabled", [](const std::string & name) {AdvancedTimer::isEnabled(name);}, py::arg("id"), doc::Timer::isEnabled);
    timer.def("setEnabled", [](const std::string & n, bool e) {AdvancedTimer::setEnabled(n, e);}, py::arg("name"), py::arg("enabled"), doc::Timer::setEnabled);
    timer.def("getInterval", [](const std::string & name) {AdvancedTimer::getInterval(name);}, py::arg("id"), doc::Timer::getInterval);
    timer.def("setInterval", [](const std::string & name, unsigned int interval) {AdvancedTimer::setInterval(name, interval);}, py::arg("id"), py::arg("interval"), doc::Timer::setInterval);

    timer.def("begin", [](const std::string& id){ AdvancedTimer::begin(id);}, py::arg("id"));
    timer.def("stepBegin", [](const std::string& id){ AdvancedTimer::stepBegin(id);}, py::arg("id"));
    timer.def("stepEnd", [](const std::string& id){ AdvancedTimer::stepEnd(id);}, py::arg("id"));
    timer.def("end", [](const std::string& id){ AdvancedTimer::end(id);}, py::arg("id"));

    timer.def("setOutputType", [](const std::string& id, const std::string& type){ AdvancedTimer::setOutputType(id, type);}, py::arg("id"), py::arg("newOutputType"), doc::Timer::setOutputType);
    timer.def("getRecords", &getRecords, py::arg("id"));

    return timer;
}

} /// namespace sofapython3

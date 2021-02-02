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

#include <sofa/helper/AdvancedTimer.h>
#include <stack>
using sofa::helper::AdvancedTimer;

#include "Submodule_Timer.h"
#include "Submodule_Timer_doc.h"

#include <stack>

namespace sofapython3
{

py::dict getRecords(const std::string & id) {
    using sofa::helper::Record;
    using sofa::helper::system::thread::ctime_t;
    using sofa::helper::system::thread::CTime;

    static auto timer_freq = CTime::getTicksPerSec();
    auto getTime = [](ctime_t t) {
        return 1000.0 * t / timer_freq;
    };

    const auto records = AdvancedTimer::getRecords(id);

    std::stack<py::dict> tokens;
    py::dict token, token_temp;
    tokens.push(token);
    ctime_t t0;

    for (const auto & r : records) {
        switch (r.type) {
            case Record::RNONE:
                break;
            case Record::RBEGIN: // Timer begins
                token = tokens.top();
                if (token.contains(r.label.c_str())) {
                    if (py::list::check_(token[r.label.c_str()])) {
                        token_temp = py::dict();
                        py::list(token[r.label.c_str()]).append(token_temp);
                        token = token_temp;
                    } else if (py::dict::check_(token[r.label.c_str()])) {
                        token_temp = token[r.label.c_str()];
                        token[r.label.c_str()] = py::list();
                        py::list(token[r.label.c_str()]).append(token_temp);
                        token_temp = py::dict();
                        py::list(token[r.label.c_str()]).append(token_temp);
                        token = token_temp;
                    } else {
                        msg_error("Timer::getRecords") << "Got an unexpected token of type '" << std::string(py::str(token.get_type())) << "'.";
                        break;
                    }
                } else {
                    token[r.label.c_str()] = py::dict();
                    token = token[r.label.c_str()];
                }
                t0 = r.time;
                token["start_time"] = getTime(r.time - t0);
                tokens.push(token);
                break;
            case Record::REND: // Timer ends
                token = tokens.top();
                token["end_time"] = getTime(r.time - t0);
                token["total_time"] = getTime(r.time - t0) - py::cast<float>(token["start_time"]);
                tokens.pop();
                break;
            case Record::RSTEP_BEGIN: // Step begins
                token = tokens.top();
                if (token.contains(r.label.c_str())) {
                    if (py::list::check_(token[r.label.c_str()])) {
                        token_temp = py::dict();
                        py::list(token[r.label.c_str()]).append(token_temp);
                        token = token_temp;
                    } else if (py::dict::check_(token[r.label.c_str()])) {
                        token_temp = token[r.label.c_str()];
                        token[r.label.c_str()] = py::list();
                        py::list(token[r.label.c_str()]).append(token_temp);
                        token_temp = py::dict();
                        py::list(token[r.label.c_str()]).append(token_temp);
                        token = token_temp;
                    } else {
                        msg_error("Timer::getRecords") << "Got an unexpected token of type '" << std::string(py::str(token.get_type())) << "'.";
                        break;
                    }
                } else {
                    token[r.label.c_str()] = py::dict();
                    token = token[r.label.c_str()];
                }
                token["start_time"] = getTime(r.time - t0);
                tokens.push(token);
                break;
            case Record::RSTEP_END: // Step ends
                token = tokens.top();
                token["end_time"] = getTime(r.time - t0);
                token["total_time"] = getTime(r.time - t0) - py::cast<float>(token["start_time"]);
                tokens.pop();
                break;
            case Record::RVAL_SET: // Sets a value
                token = tokens.top();
                token[r.label.c_str()] = r.val;
                break;
            case Record::RVAL_ADD: // Sets a value
                token = tokens.top();
                token[r.label.c_str()] = r.val;
                break;
            default:
                token = tokens.top();
                token[r.label.c_str()] = py::list();
                token = token[r.label.c_str()];
                token["start_time"] = r.time;
                break;
        }
    }

    // There should be two remaining records: Top level "record" + "timer starts". The "timer starts" record remains in
    // the stack since we normally get the records before the timer ends (ending the timer in Sofa destroys the
    // records...)
    if (tokens.size() == 2) {
        token = tokens.top();
        tokens.pop();
    } else if (tokens.size() == 1) {
        // This should not happen unless we successfully got the timer records AFTER the timer has ends, which would mean
        // that Sofa's advanced timer has improved, let not warn the user for that.
        token = tokens.top();
    }

    // Pop the last token ("records")
    tokens.pop();

    // The stack should be empty by now
    if (!tokens.empty())
        msg_error("Timer::getRecords") << "Records stack leaked.";

    return token;
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

    timer.def("setOutputType", &AdvancedTimer::setOutputType, py::arg("id"), py::arg("newOutputType"), doc::Timer::setOutputType);
    timer.def("getRecords", &getRecords, py::arg("id"));

    return timer;
}

} /// namespace sofapython3

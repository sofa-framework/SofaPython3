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
#pragma once

#include <functional>
#include <algorithm>
#include <iostream>
#include <sofa/helper/DiffLib.h>

namespace sofapython3
{

template<class Iterable, class UnaryOperation, class PickingFunction>
void fillVectorOfStringFrom(const Iterable& v, const UnaryOperation& op, const PickingFunction func)
{
    std::transform(v.begin(), v.end(), op, func);
}

template<class Iterable, class PickingFunction=std::function<const std::string(typename Iterable::value_type)> >
std::ostream& emitSpellingMessage(std::ostream& ostream, const std::string& message, const Iterable& iterable, const std::string& name,
                                  sofa::Size numEntries=5, SReal thresold=0.6_sreal,
                                  PickingFunction f = [](const typename Iterable::value_type d) { return d->getName(); })
{
    std::vector<std::string> possibleNames;
    possibleNames.reserve(iterable.size());
    fillVectorOfStringFrom(iterable, std::back_inserter(possibleNames), f);

    auto spellingSuggestions = sofa::helper::getClosestMatch(name, possibleNames, numEntries, thresold);
    if(!spellingSuggestions.empty())
    {
        for(auto& [name, score] : spellingSuggestions)
           ostream << message << "'" << name << "' ("<< std::to_string((int)(100*score))+"% match)" << std::endl;
    }
    return ostream;
}


}

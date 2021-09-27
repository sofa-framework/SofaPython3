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
#include <iostream>
#include <map>
#include <stdexcept>
#include <SofaPython3/lifetime/features.h>

namespace sofapython3::lifetime::features
{

std::map<std::string, bool> features;

bool get(const std::string& name)
{
    auto f = features.find(name);
    if(f == features.end())
        throw std::runtime_error("Missing attribute '"+name+"'");

    return (f->second);
}

void set(const std::string& name, bool value)
{
    auto f = features.find(name);
    if(f == features.end())
        throw std::runtime_error("Missing attribute '"+name+"'");

    (f->second) = value;
}

void init(const std::string& name, bool value)
{
    features[name] = value;
}


std::vector<std::string> list_features()
{
    std::vector<std::string> v;
    for(auto& it : features)
        v.push_back(it.first);
    return v;
}

}  //namespace sofapython3::futurefeatures

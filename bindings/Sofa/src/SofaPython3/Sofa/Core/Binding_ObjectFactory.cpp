/*********************************************************************
Copyright 2019, CNRS, University of Lille, INRIA

This file is part of sofaPython3

sofaPython3 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

sofaPython3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with sofaqtquick. If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/
/********************************************************************
 Contributors:
    - damien.marchal@univ-lille.fr
    - bruno.josue.marques@inria.fr
    - eve.le-guillou@centrale.centralelille.fr
    - jean-nicolas.brunet@inria.fr
    - thierry.gaugry@inria.fr
********************************************************************/

#include <sofa/core/ObjectFactory.h>
#include <SofaPython3/Sofa/Core/Binding_ObjectFactory.h>
#include <SofaPython3/Sofa/Core/Binding_ObjectFactory_doc.h>
#include <numeric>
#include <sstream>
#include <pybind11/stl.h>

using sofa::core::ObjectFactory;
namespace py { using namespace pybind11; }

namespace sofapython3 {

std::string __repr__(const ObjectFactory::ClassEntry & entry) {
    std::ostringstream s;
    s << "<" << entry.className << " at " << std::hex << &entry << ">";
    return s.str();
}

std::string __str__(const ObjectFactory::ClassEntry & entry) {
    std::string desc;
    desc += "Component '" + entry.className + "' \n";
    desc += "  Description: " + entry.description + "\n";
    desc += "  Authors: " + entry.authors + "\n";

    std::string aliases = entry.aliases.empty() ? "None" :
        std::accumulate(std::next(entry.aliases.begin()), entry.aliases.end(), *(entry.aliases.begin()),
                        [](std::string a, const std::string &b) {
                            return std::move(a) + ", " + b;
                        }
        );
    desc += "  Aliases: " + aliases + "\n";

    std::set<std::string> templates;
    std::set<std::string> targets;
    std::set<std::string> locations;
    std::string default_template = entry.defaultTemplate;
    for (const auto & creator : entry.creatorMap) {
        if (! creator.first.empty())
            templates.emplace(creator.first);
        if (creator.second) {
            if (! std::string(creator.second->getTarget()).empty())
                targets.emplace(creator.second->getTarget());
            if (! std::string(creator.second->getHeaderFileLocation()).empty())
                locations.emplace(creator.second->getHeaderFileLocation());
        }
    }
    std::string templates_str = templates.empty() ? "None" :
        std::accumulate(std::next(templates.begin()), templates.end(), "'"+*(templates.begin())+"'",
                        [&default_template](std::string a, const std::string & b) {
                            return std::move(a) + ", '" + b + (b == default_template ? "' (default)" : "'");
                        }
        );
    desc += "  Templates: " + templates_str + "\n";

    std::string targets_str = targets.empty() ? "None" :
                              std::accumulate(std::next(targets.begin()), targets.end(), *(targets.begin()),
                                              [](std::string a, const std::string &b) {
                                                  return std::move(a) + ", " + b;
                                              }
                              );
    desc += "  Targets: " + targets_str + "\n";

    std::string location_str = locations.empty() ? "None" :
                              std::accumulate(std::next(locations.begin()), locations.end(), "\n    "+*(locations.begin()),
                                              [](std::string a, const std::string &b) {
                                                  return std::move(a) + "\n    " + b;
                                              }
                              );
    desc += "  Locations: " + location_str;

    return desc;
}

std::set<std::string> getTargets(ObjectFactory& f) {
    std::set<std::string> targets;
    std::vector<ObjectFactory::ClassEntry::SPtr> entries;
    f.getAllEntries(entries);
    for (const auto & entry : entries) {
        for (const auto & creator : entry->creatorMap) {
            if (! std::string(creator.second->getTarget()).empty())
                targets.emplace(creator.second->getTarget());
        }
    }
    return targets;
}

std::set<std::string> getTemplates(const ObjectFactory::ClassEntry &entry) {
    std::set<std::string> templates;
    for (const auto & creator : entry.creatorMap) {
        if (! creator.first.empty())
            templates.emplace(creator.first);
    }
    return templates;
}

std::set<std::string> getTargetsOfEntry(const ObjectFactory::ClassEntry &entry) {
    std::set<std::string> targets;
    for (const auto & creator : entry.creatorMap) {
        if (! creator.first.empty())
            if (creator.second && ! std::string(creator.second->getTarget()).empty()) {
                targets.emplace(creator.second->getTarget());
            }
    }
    return targets;
}

std::set<std::string> getLocationsOfEntry(const ObjectFactory::ClassEntry &entry) {
    std::set<std::string> locations;
    for (const auto & creator : entry.creatorMap) {
        if (! creator.first.empty())
            if (creator.second && ! std::string(creator.second->getHeaderFileLocation()).empty()) {
                locations.emplace(creator.second->getHeaderFileLocation());
            }
    }
    return locations;
}

std::string className(const ObjectFactory::ClassEntry &e) { return e.className; }

py::list aliases(const ObjectFactory::ClassEntry &self)
{
    py::list list;
    for (auto alias : self.aliases)
        list.append(alias);
    return list;
}

std::string description(const ObjectFactory::ClassEntry &e) { return e.description; }

std::string authors(const ObjectFactory::ClassEntry &e) { return e.authors; }

std::string license(const ObjectFactory::ClassEntry &e) { return e.license; }

std::string defaultTemplate(const ObjectFactory::ClassEntry &e) { return e.defaultTemplate; }

py::dict dataAlias(const ObjectFactory::ClassEntry &self)
{
    py::dict dict;
    for (auto alias : self.m_dataAlias)
    {
        py::list aliases;
        for (auto a : alias.second)
            aliases.append(a);
        dict[alias.first.c_str()] = aliases;
    }
    return dict;
}


void moduleAddObjectFactory(py::module &m) {
    py::class_<ObjectFactory> factory (m, "ObjectFactory", sofapython3::doc::objectmodel::ObjectFactoryClass);

    py::class_<ObjectFactory::ClassEntry> entry(m, sofapython3::doc::objectmodel::ClassEntryClass);
    entry.def_property_readonly("className", &className);
    entry.def_property_readonly("aliases", &aliases);
    entry.def_property_readonly("description", &description);
    entry.def_property_readonly("authors", &authors);
    entry.def_property_readonly("license", &license);
    entry.def_property_readonly("defaultTemplate", &defaultTemplate);
    entry.def_property_readonly("dataAlias", &dataAlias);
    entry.def_property_readonly("templates", &getTemplates);
    entry.def_property_readonly("targets", &getTargetsOfEntry);
    entry.def_property_readonly("locations", &getLocationsOfEntry);

    entry.def("__str__", &__str__);
    entry.def("__repr__", &__repr__);


    factory.def_static("getComponent", [](const std::string & component_name) {
        return ObjectFactory::getInstance()->getEntry(component_name);
    }, doc::objectmodel::ObjectFactory_getEntry);

    factory.def_static("shortName", [](const std::string & component_name) {
        return ObjectFactory::getInstance()->shortName(component_name);
    }, doc::objectmodel::ObjectFactory_shortName);

    factory.def_property_readonly_static("components", [](const py::object &) {
        std::vector<ObjectFactory::ClassEntry::SPtr> entries;
        ObjectFactory::getInstance()->getAllEntries(entries);
        // Copy all entries since the pointers will be deleted
        std::vector<ObjectFactory::ClassEntry> returned_entries;
        returned_entries.reserve(entries.size());
        for (const ObjectFactory::ClassEntry::SPtr & entry : entries) {
            returned_entries.push_back(*entry);
        }
        return returned_entries;
    }, doc::objectmodel::ObjectFactory_getAllEntries);

    factory.def_static("getComponentsFromTarget", [](const std::string & target_name) {
        std::vector<ObjectFactory::ClassEntry::SPtr> entries;
        ObjectFactory::getInstance()->getEntriesFromTarget(entries, target_name);
        // Copy all entries since the pointers will be deleted
        std::vector<ObjectFactory::ClassEntry> returned_entries;
        returned_entries.reserve(entries.size());
        for (const ObjectFactory::ClassEntry::SPtr & entry : entries) {
            returned_entries.push_back(*entry);
        }
        return returned_entries;
    }, doc::objectmodel::ObjectFactory_getEntriesFromTarget);

    factory.def_property_readonly_static("targets", [](const py::object &){
        return getTargets(*ObjectFactory::getInstance());
    }, doc::objectmodel::ObjectFactory_targets);
}

} /// namespace sofapython3

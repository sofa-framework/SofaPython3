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

#include <SofaPython3/Sofa/Core/Binding_DataDict.h>

using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::BaseData;
using sofa::core::sptr;

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

namespace sofapython3 {

void moduleAddDataDict(py::module& m)
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// DataDict binding
    ////////////////////////////////////////////////////////////////////////////////////////////////
    py::class_<DataDict> d(m, "DataDict",
                           sofapython3::doc::dataDict::Class);
    d.def("__len__", [](DataDict& self)
    {
        return self.owner->getDataFields().size();
    });


    d.def("__getitem__",[](DataDict& self, const size_t& i)
    {
        const Base::VecData& vd = self.owner->getDataFields();
        if(i > vd.size())
            throw py::index_error(std::to_string(i));
        return PythonFactory::toPython(vd[i]);
    });

    d.def("__getitem__",
          [](DataDict& self, const std::string& s) -> py::object
    {
        BaseData* d = self.owner->findData(s);
        if(d!=nullptr)
            return py::cast(d);
        throw std::invalid_argument(s);
    });

    ////////////////////////////////////////////////////////////////////////////////////////////////
    d.def("__setitem__",[](DataDict& d, const std::string& s, py::object v)
    {
        return 0.0;
    });

    d.def("__iter__", [](DataDict& d)
    {
        return DataDictIterator(d.owner, true, false);
    });
    d.def("keys", [](DataDict& d)
    {
        return DataDictIterator(d.owner, true, false);
    }, sofapython3::doc::dataDict::keys);
    d.def("values", [](DataDict& d)
    {
        return DataDictIterator(d.owner, false, true);
    }, sofapython3::doc::dataDict::values);
    d.def("items", [](DataDict& d)
    {
        return DataDictIterator(d.owner, true, true);
    }, sofapython3::doc::dataDict::items);
}


void moduleAddDataDictIterator(py::module &m)
{
    PythonFactory::registerType<sofa::core::objectmodel::Base>(
                [](sofa::core::objectmodel::Base* object)
    {
        return py::cast(object);
    });


    py::class_<DataDictIterator> ddi(m, "DataDictIterator", sofapython3::doc::dataDictIterator::Class);
    ddi.def("__iter__", [](DataDictIterator& d)
    {
        return d;
    });
    ddi.def("__next__", [](DataDictIterator& d) -> py::object
    {
        if(d.index>=d.owner->getDataFields().size())
            throw py::stop_iteration();

        BaseData* data = d.owner->getDataFields()[d.index++];
        if(!d.key)
            return PythonFactory::toPython(data);

        if(!d.value)
            return py::cast(data->getName());

        py::tuple t {2};
        t[0] = data->getName();
        t[1] = PythonFactory::toPython(data);
        return std::move(t);
    });
}

}  // namespace sofapython3

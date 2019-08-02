#include "Binding_DataDict.h"

namespace sofapython3
{
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
        return toPython(vd[i]);
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


    py::class_<DataDictIterator> ddi(m, "DataDictIterator");
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
            return toPython(data);

        if(!d.value)
            return py::cast(data->getName());

        py::tuple t {2};
        t[0] = data->getName();
        t[1] = toPython(data);
        return std::move(t);
    });
}

}  // namespace sofapython3

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
    - paul.scheikl@kit.edu
********************************************************************/


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaBaseCollision/ContactListener.h>

#include <SofaPython3/Sofa/Core/Binding_ContactListener.h>
#include <SofaPython3/Sofa/Core/Binding_ContactListener_doc.h>


#include <SofaPython3/PythonFactory.h>

namespace py { using namespace pybind11; }
using sofa::core::objectmodel::BaseObject;
using sofa::core::collision::ContactListener;

namespace sofapython3
{

    void hello(const ContactListener& self){
        std::cout << "Hello \n";
    }


void moduleAddContactListener(pybind11::module &m)
{
    /* /// register the ContactListener binding in the pybind11 typing sytem */
    pybind11::class_<ContactListener,
               sofa::core::objectmodel::BaseObject,
               py_shared_ptr<ContactListener>> c(m, "ContactListener", sofapython3::doc::contactListener::contactListenerClass);

    /// register the ContactListener binding in the downcasting subsystem
    PythonFactory::registerType<ContactListener>([](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<ContactListener*>(object));
    });
    c.def("hello", &hello);
    c.def("getNumberOfContacts", &ContactListener::getNumberOfContacts);
}

} /// namespace sofapython3

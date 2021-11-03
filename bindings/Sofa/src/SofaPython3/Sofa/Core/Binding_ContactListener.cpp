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


#include "sofa/config.h"
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

void moduleAddContactListener(pybind11::module &m)
{
    /// register the ContactListener binding in the pybind11 typing sytem
    pybind11::class_<ContactListener,
               sofa::core::objectmodel::BaseObject,
               py_shared_ptr<ContactListener>> c(m, "ContactListener", sofapython3::doc::contactListener::contactListenerClass);

    /// Commented out until feature is in SOFA master
    /* c.def("getNumberOfContacts", &ContactListener::getNumberOfContacts); */

    /// register the ContactListener binding in the downcasting subsystem
    PythonFactory::registerType<ContactListener>([](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<ContactListener*>(object));
    });
}

} /// namespace sofapython3

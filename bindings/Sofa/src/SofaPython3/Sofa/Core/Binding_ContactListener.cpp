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
#include <sofa/component/collision/response/contact/ContactListener.h>

#include <SofaPython3/Sofa/Core/Binding_ContactListener.h>
#include <SofaPython3/Sofa/Core/Binding_ContactListener_doc.h>


#include <SofaPython3/PythonFactory.h>

namespace py { using namespace pybind11; }
using sofa::core::objectmodel::BaseObject;
using sofa::component::collision::response::contact::ContactListener;

namespace sofapython3
{

    pybind11::dict getContactData(const ContactListener& listener){
        pybind11::dict contactData;
        const sofa::Size numberOfContacts = listener.getNumberOfContacts();
        contactData["numberOfContacts"] = numberOfContacts;

        const std::vector<std::tuple<unsigned int, unsigned int, unsigned int, unsigned int>> contactElements = listener.getContactElements();
        const std::vector<std::tuple<unsigned int, sofa::type::Vec3, unsigned int, sofa::type::Vec3>> contactPoints = listener.getContactPoints();

        std::vector<unsigned int> collisionElementsModel1;
        std::vector<unsigned int> collisionElementsModel2;
        collisionElementsModel1.reserve(numberOfContacts);
        collisionElementsModel2.reserve(numberOfContacts);

        std::vector<sofa::type::Vec3> collisionPointsModel1;
        std::vector<sofa::type::Vec3> collisionPointsModel2;
        collisionPointsModel1.reserve(numberOfContacts);
        collisionPointsModel2.reserve(numberOfContacts);

        for (unsigned int i = 0; i < numberOfContacts; i++){

            if(std::get<0>(contactElements[i]) == 0){
                collisionElementsModel1.emplace_back(std::get<1>(contactElements[i]));
                collisionElementsModel2.emplace_back(std::get<3>(contactElements[i]));
            }
            else{
                collisionElementsModel1.emplace_back(std::get<3>(contactElements[i]));
                collisionElementsModel2.emplace_back(std::get<1>(contactElements[i]));
            }

            if(std::get<0>(contactPoints[i]) == 0){
                collisionPointsModel1.emplace_back(std::get<1>(contactPoints[i]));
                collisionPointsModel2.emplace_back(std::get<3>(contactPoints[i]));
            }
            else{
                collisionPointsModel1.emplace_back(std::get<3>(contactPoints[i]));
                collisionPointsModel2.emplace_back(std::get<1>(contactPoints[i]));
            }
        }

        contactData["collisionPointsModel1"] = collisionPointsModel1;
        contactData["collisionPointsModel2"] = collisionPointsModel2;

        contactData["collisionElementsModel1"] = collisionElementsModel1;
        contactData["collisionElementsModel2"]= collisionElementsModel2;

        return contactData;
    }


void moduleAddContactListener(pybind11::module &m)
{
    /// register the ContactListener binding in the pybind11 typing sytem
    pybind11::class_<ContactListener,
               sofa::core::objectmodel::BaseObject,
               py_shared_ptr<ContactListener>> c(m, "ContactListener", sofapython3::doc::contactListener::contactListenerClass);

    c.def("getNumberOfContacts", &ContactListener::getNumberOfContacts);
    c.def("getContactData", &getContactData);
    c.def("getDistances", &ContactListener::getDistances);
    c.def("getContactPoints", &ContactListener::getContactPoints);
    c.def("getContactElements", &ContactListener::getContactElements);

    /// register the ContactListener binding in the downcasting subsystem
    PythonFactory::registerType<ContactListener>([](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<ContactListener*>(object));
    });
}

} /// namespace sofapython3

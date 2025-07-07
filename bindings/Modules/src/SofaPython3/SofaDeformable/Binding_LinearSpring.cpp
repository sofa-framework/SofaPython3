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

#include <pybind11/pybind11.h>

#include <sofa/component/solidmechanics/spring/SpringForceField.h>

#include <SofaPython3/SofaDeformable/Binding_LinearSpring.h>
#include <SofaPython3/SofaDeformable/Binding_LinearSpring_doc.h>


namespace sofapython3 {

using sofa::component::solidmechanics::spring::LinearSpring;
typedef LinearSpring<SReal> LinearSpringR;

namespace py { using namespace pybind11; }

void moduleAddLinearSpring(pybind11::module& m) {
    // create a python binding for the C++ class LinearSpring from SofaDeformable
    py::class_<LinearSpringR> s (m, "LinearSpring", sofapython3::doc::SofaDeformable::LinearSpringClass);

    // initializer for the class
    s.def(py::init<sofa::Index,
                   sofa::Index,
                   SReal,
                   SReal,
                   SReal,
                   bool,
                   bool>(),
                   py::arg("index1"),
                   py::arg("index2"),
                   py::arg("springStiffness") = 100,
                   py::arg("dampingFactor") = 5,
                   py::arg("restLength"),
                   py::arg("elongationOnly") = false,
                   py::arg("enabled") = true,
                   sofapython3::doc::SofaDeformable::LinearSpringInit);

    // make class fields accessible from python (e.g. spring.restLength = 10)
    s.def_readwrite("index1", &LinearSpringR::m1);
    s.def_readwrite("index2", &LinearSpringR::m2);
    s.def_readwrite("springStiffness", &LinearSpringR::ks);
    s.def_readwrite("dampingFactor", &LinearSpringR::kd);
    s.def_readwrite("restLength", &LinearSpringR::initpos);
    s.def_readwrite("elongationOnly", &LinearSpringR::elongationOnly);
    s.def_readwrite("enabled", &LinearSpringR::enabled);

    // print the spring in a human readable form in python with print(spring)
    s.def("__str__", [](const LinearSpringR& self){
      std::ostringstream out;
      out << "<Sofa.SofaDeformable.LinearSpring: [";
      out << "index1: ";
      out << self.m1;
      out << "; index2: ";
      out << self.m2;
      out << "; springStiffness: ";
      out << self.ks;
      out << "; dampingFactor: ";
      out << self.kd;
      out << "; restLength: ";
      out << self.initpos;
      out << "; elongationOnly: ";
      out << self.elongationOnly;
      out << "; enabled: ";
      out << self.enabled;
      out << "] at ";
      out << &self;
      out << ">";

      return out.str();
    });

}

} // namespace sofapython3

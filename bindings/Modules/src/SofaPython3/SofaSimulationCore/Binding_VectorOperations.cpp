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

#include <sofa/simulation/config.h>
#include <sofa/simulation/VectorOperations.h>

namespace py { using namespace pybind11; }

namespace sofapython3 {

void moduleAddVectorOperations(pybind11::module& m)
{
    using namespace sofa::simulation::common;

    py::class_<VectorOperations> c (m, "VectorOperations");
    c.def(py::init([](sofa::core::objectmodel::BaseContext* context, bool precomputedTraversalOrder = false) {
        return VectorOperations(sofa::core::ExecParams::defaultInstance(), context, precomputedTraversalOrder);
    }), py::arg("context"), py::arg("precomputedTraversalOrder") = false);

    c.def("v_alloc", py::overload_cast<sofa::core::MultiVecCoordId&>(&VectorOperations::v_alloc), py::arg("multi_vector_id"));
    c.def("v_alloc", py::overload_cast<sofa::core::MultiVecDerivId&>(&VectorOperations::v_alloc), py::arg("multi_vector_id"));

    c.def("v_free", py::overload_cast<sofa::core::MultiVecCoordId&, bool, bool>(&VectorOperations::v_free), py::arg("multi_vector_id"), py::arg("interactionForceField") = false, py::arg("propagate") = false);
    c.def("v_free", py::overload_cast<sofa::core::MultiVecDerivId&, bool, bool>(&VectorOperations::v_free), py::arg("multi_vector_id"), py::arg("interactionForceField") = false, py::arg("propagate") = false);

    c.def("v_realloc", py::overload_cast<sofa::core::MultiVecCoordId&, bool, bool>(&VectorOperations::v_realloc), py::arg("multi_vector_id"), py::arg("interactionForceField") = false, py::arg("propagate") = false);
    c.def("v_realloc", py::overload_cast<sofa::core::MultiVecDerivId&, bool, bool>(&VectorOperations::v_realloc), py::arg("multi_vector_id"), py::arg("interactionForceField") = false, py::arg("propagate") = false);

    c.def("v_clear", &VectorOperations::v_clear, py::arg("multi_vector_id"));
    c.def("v_size", &VectorOperations::v_size, py::arg("multi_vector_id"));

    c.def("v_eq",  py::overload_cast<sofa::core::MultiVecId, sofa::core::ConstMultiVecId>(&VectorOperations::v_eq), py::arg("v").noconvert(false), py::arg("a").noconvert(false), "v = a");
    c.def("v_eq", [](VectorOperations & self, sofa::core::MultiVecId &v, sofa::core::ConstMultiVecId & a){
        self.v_eq(v, a);
    });
    c.def("v_eq",  py::overload_cast<sofa::core::MultiVecId, sofa::core::ConstMultiVecId, SReal>(&VectorOperations::v_eq), py::arg("v").noconvert(false), py::arg("a").noconvert(false), py::arg("f").noconvert(false), "v = f*a");
    c.def("v_peq",  &VectorOperations::v_peq, py::arg("v"), py::arg("a"), py::arg("f")=1.0, "v += f*a");
    c.def("v_teq",  &VectorOperations::v_teq, py::arg("v"), py::arg("f"), "v *= f");
    c.def("v_op",  &VectorOperations::v_op, py::arg("v"), py::arg("a"), py::arg("b"), py::arg("f"), "v=a+b*f");

    c.def("v_dot", &VectorOperations::v_dot, py::arg("a"), py::arg("b"), "a dot b ( get result using finish )");
    c.def("v_norm", &VectorOperations::v_norm, py::arg("a"), py::arg("l"), "Compute the norm of a vector ( get result using finish ). The type of norm is set by parameter l. Use 0 for the infinite norm. Note that the 2-norm is more efficiently computed using the square root of the dot product.");
    c.def("finish", &VectorOperations::finish);

    c.def("v_threshold", &VectorOperations::v_threshold, py::arg("a"), py::arg("threshold"), "Nullify the values below the given threshold.");
}

}
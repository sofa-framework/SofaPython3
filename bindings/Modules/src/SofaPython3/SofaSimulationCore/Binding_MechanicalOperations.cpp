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

#include <sofa/simulation/MechanicalOperations.h>

namespace py { using namespace pybind11; }

namespace sofapython3 {

void moduleAddMechanicalOperations(pybind11::module& m)
{
    using namespace sofa::simulation::common;

    py::class_<MechanicalOperations> c (m, "MechanicalOperations");
    c.def(py::init<const sofa::core::MechanicalParams*, sofa::core::objectmodel::BaseContext*, bool>(), py::arg("mparams"), py::arg("ctx"), py::arg("precomputedTraversalOrder")=false);

    // Mechanical Vector operations
    c.def("propagateDx", &MechanicalOperations::propagateDx, py::arg("dx"), py::arg("ignore_flag") = false, "Propagate the given displacement through all mappings");
    c.def("propagateDxAndResetDf", &MechanicalOperations::propagateDxAndResetDf, py::arg("dx"), py::arg("df"), "Propagate the given displacement through all mappings and reset the current force delta");
    c.def("propagateX", &MechanicalOperations::propagateX, py::arg("x"), "Propagate the given position through all mappings");
    c.def("propagateV", &MechanicalOperations::propagateV, py::arg("v"), "Propagate the given velocity through all mappings");
    c.def("propagateXAndV", &MechanicalOperations::propagateXAndV, py::arg("x"), py::arg("v"), "Propagate the given position and velocity through all mappings");
    c.def("propagateXAndResetF", &MechanicalOperations::propagateXAndResetF, py::arg("x"), py::arg("f"), "Propagate the given position through all mappings and reset the current force delta");
    c.def("projectPosition", &MechanicalOperations::projectPosition, py::arg("x"), py::arg("time") = 0., "Apply projective constraints to the given position vector");
    c.def("projectVelocity", &MechanicalOperations::projectVelocity, py::arg("v"), py::arg("time") = 0., "Apply projective constraints to the given velocity vector");
    c.def("projectResponse", [](MechanicalOperations & self, sofa::core::MultiVecDerivId dx) {
        self.projectResponse(dx);
    }, py::arg("dx"), "Apply projective constraints to the given vector");
    c.def("projectPositionAndVelocity", &MechanicalOperations::projectPositionAndVelocity, py::arg("x"), py::arg("v"), py::arg("time") = 0., "Apply projective constraints to the given position and velocity vectors");
    c.def("addMdx", &MechanicalOperations::addMdx, py::arg("res"), py::arg("dx"), py::arg("factor") = 1.0, "res += factor M.dx");
    c.def("integrateVelocity", &MechanicalOperations::integrateVelocity, py::arg("res"), py::arg("x"), py::arg("v"), py::arg("dt"), "res = x + v.dt");
    c.def("accFromF", &MechanicalOperations::accFromF, py::arg("a"), py::arg("f"), "a = M^-1 . f");
    c.def("computeEnergy", &MechanicalOperations::computeEnergy, py::arg("kineticEnergy"), py::arg("potentialEnergy"), "Compute Energy");
    c.def("computeForce", py::overload_cast<sofa::core::MultiVecDerivId, bool, bool, bool>(&MechanicalOperations::computeForce), py::arg("result"), py::arg("clear")=true, py::arg("accumulate")=true, py::arg("neglectingCompliance")=true, "Compute the current force (given the latest propagated position and velocity)");
    c.def("computeForce", py::overload_cast<SReal, sofa::core::MultiVecDerivId, sofa::core::MultiVecCoordId, sofa::core::MultiVecDerivId, bool>(&MechanicalOperations::computeForce), py::arg("t"), py::arg("f"), py::arg("x"), py::arg("v"), py::arg("neglectingCompliance")=true, "Compute f(x,v) at time t. Parameters x and v not const due to propagation through mappings.");
    c.def("computeDf", &MechanicalOperations::computeDf, py::arg("df"), py::arg("clear")=true, py::arg("accumulate")=true, "Compute the current force delta (given the latest propagated displacement)");
    c.def("computeDfV", &MechanicalOperations::computeDfV, py::arg("df"), py::arg("clear")=true, py::arg("accumulate")=true, "Compute the current force delta (given the latest propagated velocity)");
    c.def("addMBKdx", &MechanicalOperations::addMBKdx, py::arg("df"), py::arg("m"), py::arg("b"), py::arg("k"), py::arg("clear")=true, py::arg("accumulate")=true, "accumulate $ df += (m M + b B + k K) dx $ (given the latest propagated displacement)");
    c.def("addMBKv", &MechanicalOperations::addMBKv, py::arg("df"), py::arg("m"), py::arg("b"), py::arg("k"), py::arg("clear")=true, py::arg("accumulate")=true, "accumulate $ df += (m M + b B + k K) velocity $");
    c.def("addSeparateGravity", &MechanicalOperations::addSeparateGravity, py::arg("dt"), py::arg("result") = sofa::core::VecDerivId::velocity(), "Add dt*Gravity to the velocity");
    c.def("computeContactForce", &MechanicalOperations::computeContactForce, py::arg("result"));
    c.def("computeContactDf", &MechanicalOperations::computeContactDf, py::arg("df"));
    c.def("computeAcc", &MechanicalOperations::computeAcc, py::arg("t"), py::arg("a"), py::arg("x"), py::arg("v"), "Compute a(x,v) at time t. Parameters x and v not const due to propagation through mappings.");
    c.def("computeContactAcc", &MechanicalOperations::computeContactAcc, py::arg("t"), py::arg("a"), py::arg("x"), py::arg("v"), "Parameters x and v not const due to propagation through mappings.");

    // Matrix operations using LinearSolver components
    c.def("m_resetSystem", &MechanicalOperations::m_resetSystem);
    c.def("m_setSystemMBKMatrix", &MechanicalOperations::m_setSystemMBKMatrix, py::arg("m"), py::arg("b"), py::arg("k"));
    c.def("m_setSystemRHVector", &MechanicalOperations::m_setSystemRHVector, py::arg("f"));
    c.def("m_setSystemLHVector", &MechanicalOperations::m_setSystemLHVector, py::arg("dx"));
    c.def("m_solveSystem", &MechanicalOperations::m_solveSystem);

    // Constraints
    c.def("solveConstraint", &MechanicalOperations::solveConstraint, py::arg("id"), py::arg("order"));

    // Matrix operations
    c.def("getMatrixDimension", py::overload_cast<sofa::Size*, sofa::Size*, sofa::core::behavior::MultiMatrixAccessor*>(&MechanicalOperations::getMatrixDimension), py::arg("nrows"), py::arg("ncols"), py::arg("matrix") = nullptr);
    c.def("getMatrixDimension", py::overload_cast<sofa::core::behavior::MultiMatrixAccessor*>(&MechanicalOperations::getMatrixDimension), py::arg("matrix"));
    c.def("addMBK_ToMatrix", &MechanicalOperations::addMBK_ToMatrix, py::arg("matrix"), py::arg("mFact"), py::arg("bFact"), py::arg("kFact"));
    c.def("multiVector2BaseVector", &MechanicalOperations::multiVector2BaseVector, py::arg("src"), py::arg("dest"), py::arg("matrix"));
    c.def("baseVector2MultiVector", &MechanicalOperations::baseVector2MultiVector, py::arg("src"), py::arg("dest"), py::arg("matrix"));
    c.def("multiVectorPeqBaseVector", &MechanicalOperations::multiVectorPeqBaseVector, py::arg("dest"), py::arg("src"), py::arg("matrix"));
}

}

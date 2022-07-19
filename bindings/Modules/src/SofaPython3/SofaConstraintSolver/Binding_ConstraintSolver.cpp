/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2021 INRIA, USTL, UJF, CNRS, MGH                     *
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
#include <SofaPython3/SofaConstraintSolver/Binding_ConstraintSolver.h>
#include <SofaPython3/SofaConstraintSolver/Binding_ConstraintSolver_doc.h>
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/PythonFactory.h>

#include <sofa/component/constraint/lagrangian/solver/ConstraintSolverImpl.h>

namespace py { using namespace pybind11; }

namespace sofapython3 {

using sofa::component::constraint::lagrangian::solver::ConstraintSolverImpl;
using EigenDenseMatrix = Eigen::Matrix<SReal, Eigen::Dynamic, Eigen::Dynamic>;
using EigenMatrixMap = Eigen::Map<EigenDenseMatrix>;

void moduleAddConstraintSolver(py::module &m)
{
    const auto typeName = ConstraintSolverImpl::GetClass()->className;
    py::class_<ConstraintSolverImpl,
               sofa::core::objectmodel::BaseObject,
               sofapython3::py_shared_ptr<ConstraintSolverImpl> > c(m, typeName.c_str(), sofapython3::doc::constraintsolver::constraintSolverClass);

    c.def("W", [](ConstraintSolverImpl& self) -> EigenMatrixMap
    {
        assert(self.getConstraintProblem());
        auto& W_matrix = self.getConstraintProblem()->W;
        return { W_matrix.ptr(), W_matrix.rows(), W_matrix.cols()};
    }, sofapython3::doc::constraintsolver::constraintSolver_W);

    c.def("lambda_force", [](ConstraintSolverImpl& self) -> Eigen::Map<Eigen::Matrix<SReal, Eigen::Dynamic, 1> >
    {
        assert(self.getConstraintProblem());
        auto& lambda = self.getConstraintProblem()->f;
        return { lambda.ptr(), lambda.size()};
    }, sofapython3::doc::constraintsolver::constraintSolver_lambda);

    /// register the binding in the downcasting subsystem
    PythonFactory::registerType<ConstraintSolverImpl>([](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<ConstraintSolverImpl*>(object));
    });
}

}

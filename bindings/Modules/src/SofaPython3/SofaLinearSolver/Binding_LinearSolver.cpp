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
#include <SofaPython3/SofaLinearSolver/Binding_LinearSolver.h>
#include <SofaPython3/SofaLinearSolver/Binding_LinearSolver_doc.h>
#include <pybind11/pybind11.h>

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/PythonFactory.h>

#include <sofa/component/linearsolver/iterative/MatrixLinearSolver.h>
#include <pybind11/eigen.h>

namespace py { using namespace pybind11; }

namespace sofapython3 {

using EigenSparseMatrix = Eigen::SparseMatrix<SReal, Eigen::RowMajor>;
using EigenMatrixMap = Eigen::Map<Eigen::SparseMatrix<SReal, Eigen::RowMajor> >;
using Vector = Eigen::Matrix<SReal,Eigen::Dynamic, 1>;
using EigenVectorMap = Eigen::Map<Vector>;

template<class TBlock>
EigenSparseMatrix toEigen(sofa::linearalgebra::CompressedRowSparseMatrix<TBlock>& matrix)
{
    sofa::linearalgebra::CompressedRowSparseMatrix<typename TBlock::Real> filtered;
    filtered.copyNonZeros(matrix);
    filtered.compress();
    return EigenMatrixMap(filtered.rows(), filtered.cols(), filtered.getColsValue().size(),
            (EigenMatrixMap::StorageIndex*)filtered.rowBegin.data(), (EigenMatrixMap::StorageIndex*)filtered.colsIndex.data(), filtered.colsValue.data());
}

template<>
EigenSparseMatrix toEigen<SReal>(sofa::linearalgebra::CompressedRowSparseMatrix<SReal>& matrix)
{
    return EigenMatrixMap(matrix.rows(), matrix.cols(), matrix.getColsValue().size(),
                    (EigenMatrixMap::StorageIndex*)matrix.rowBegin.data(), (EigenMatrixMap::StorageIndex*)matrix.colsIndex.data(), matrix.colsValue.data());
}

template<class TBlock>
void bindLinearSolvers(py::module &m)
{
    using CRS = sofa::linearalgebra::CompressedRowSparseMatrix<TBlock>;
    using CRSLinearSolver = sofa::component::linearsolver::MatrixLinearSolver<CRS, sofa::linearalgebra::FullVector<SReal> >;

    const std::string typeName = CRSLinearSolver::GetClass()->className + CRSLinearSolver::GetCustomTemplateName();

    py::class_<CRSLinearSolver,
               sofa::core::objectmodel::BaseObject,
               sofapython3::py_shared_ptr<CRSLinearSolver> > c(m, typeName.c_str(), sofapython3::doc::linearsolver::linearSolverClass);

    c.def("A", [](CRSLinearSolver& self) -> EigenSparseMatrix
    {
        if (CRS* matrix = self.l_linearSystem->getSystemMatrix())
        {
            return toEigen(*matrix);
        }
        return {};
    }, sofapython3::doc::linearsolver::linearSolver_A);

    c.def("b", [](CRSLinearSolver& self) -> Vector
    {
        if (auto* vector = self.l_linearSystem->getRHSVector())
        {
            return EigenVectorMap(vector->ptr(), vector->size());
        }
        return {};
    }, sofapython3::doc::linearsolver::linearSolver_b);

    c.def("x", [](CRSLinearSolver& self) -> Vector
    {
        if (auto* vector = self.l_linearSystem->getSolutionVector())
        {
            return EigenVectorMap(vector->ptr(), vector->size());
        }
        return {};
    }, sofapython3::doc::linearsolver::linearSolver_x);


    /// register the binding in the downcasting subsystem
    PythonFactory::registerType<CRSLinearSolver>([](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<CRSLinearSolver*>(object));
    });
}

void moduleAddLinearSolver(py::module &m)
{
    bindLinearSolvers<SReal>(m);
    bindLinearSolvers<sofa::type::Mat<3, 3, SReal> >(m);
}

}

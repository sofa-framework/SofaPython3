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
#include <SofaPython3/SofaLinearSystem/Binding_LinearSystem.inl>


#include <pybind11/eigen.h>
#include <sofa/linearalgebra/BTDMatrix.h>
#include <sofa/linearalgebra/BlockVector.h>
#include <sofa/linearalgebra/CompressedRowSparseMatrix.h>


namespace py { using namespace pybind11; }

namespace sofapython3 {

template<class Real>
using EigenSparseMatrix = Eigen::SparseMatrix<Real, Eigen::RowMajor>;

template<class Real>
using EigenMatrixMap = Eigen::Map<Eigen::SparseMatrix<Real, Eigen::RowMajor> >;

template<class TBlock>
EigenSparseMatrix<typename sofa::linearalgebra::CompressedRowSparseMatrix<TBlock>::Real>
toEigen(sofa::linearalgebra::CompressedRowSparseMatrix<TBlock>& matrix)
{
    using Real = typename sofa::linearalgebra::CompressedRowSparseMatrix<TBlock>::Real;
    if constexpr (std::is_same_v<TBlock, Real>)
    {
        matrix.compress();
        return EigenMatrixMap<Real>(matrix.rows(), matrix.cols(), matrix.getColsValue().size(),
                        (typename EigenMatrixMap<Real>::StorageIndex*)matrix.rowBegin.data(),
                        (typename EigenMatrixMap<Real>::StorageIndex*)matrix.colsIndex.data(),
                        matrix.colsValue.data());
    }
    else
    {
        sofa::linearalgebra::CompressedRowSparseMatrix<typename TBlock::Real> filtered;
        filtered.copyNonZeros(matrix);
        filtered.compress();
        return EigenMatrixMap<Real>(filtered.rows(), filtered.cols(), filtered.getColsValue().size(),
                (typename EigenMatrixMap<Real>::StorageIndex*)filtered.rowBegin.data(),
                (typename EigenMatrixMap<Real>::StorageIndex*)filtered.colsIndex.data(),
                filtered.colsValue.data());
    }
}

template<class TMatrix, class TVector>
void bindMatrixAccessCRS(LinearSystemClass<TMatrix, TVector>& c)
{
    using Real = typename TMatrix::Real;
    using CRSLinearSystem = sofa::component::linearsystem::TypedMatrixLinearSystem<TMatrix, TVector>;

    const auto matrixAccess =
        [](CRSLinearSystem& self) -> EigenSparseMatrix<Real>
        {
            if (TMatrix* matrix = self.getSystemMatrix())
            {
                if (matrix->colsValue.empty()) //null matrix: contains no entries
                {
                    return EigenSparseMatrix<Real>{matrix->rows(), matrix->cols()};
                }
                return toEigen(*matrix);
            }
            return {};
        };

    c.def("A", matrixAccess, sofapython3::doc::linearsystem::linearSystem_CRS_A);
    c.def("get_system_matrix", matrixAccess, sofapython3::doc::linearsystem::linearSystem_CRS_get_system_matrix);
}

template<>
void bindMatrixAccess(LinearSystemClass<sofa::linearalgebra::CompressedRowSparseMatrix<SReal>, sofa::linearalgebra::FullVector<SReal>>& c)
{
    bindMatrixAccessCRS(c);
}

template<>
void bindMatrixAccess(LinearSystemClass<sofa::linearalgebra::CompressedRowSparseMatrix<sofa::type::Mat<2,2,SReal>>, sofa::linearalgebra::FullVector<SReal>>& c)
{
    bindMatrixAccessCRS(c);
}

template<>
void bindMatrixAccess(LinearSystemClass<sofa::linearalgebra::CompressedRowSparseMatrix<sofa::type::Mat<3,3,SReal>>, sofa::linearalgebra::FullVector<SReal>>& c)
{
    bindMatrixAccessCRS(c);
}

template<>
void bindMatrixAccess(LinearSystemClass<sofa::linearalgebra::CompressedRowSparseMatrix<sofa::type::Mat<4,4,SReal>>, sofa::linearalgebra::FullVector<SReal>>& c)
{
    bindMatrixAccessCRS(c);
}

template<>
void bindMatrixAccess(LinearSystemClass<sofa::linearalgebra::CompressedRowSparseMatrix<sofa::type::Mat<6,6,SReal>>, sofa::linearalgebra::FullVector<SReal>>& c)
{
    bindMatrixAccessCRS(c);
}

template<>
void bindMatrixAccess(LinearSystemClass<sofa::linearalgebra::CompressedRowSparseMatrix<sofa::type::Mat<8,8,SReal>>, sofa::linearalgebra::FullVector<SReal>>& c)
{
    bindMatrixAccessCRS(c);
}

template<class Real>
using DenseMatrix = Eigen::Matrix<Real,Eigen::Dynamic, Eigen::Dynamic>;

template<class Real>
using EigenDenseMatrixMap = Eigen::Map<DenseMatrix<Real>>;

template<>
void bindMatrixAccess(LinearSystemClass<sofa::linearalgebra::FullMatrix<SReal>, sofa::linearalgebra::FullVector<SReal>>& c)
{
    using CRSLinearSystem = sofa::component::linearsystem::TypedMatrixLinearSystem<sofa::linearalgebra::FullMatrix<SReal>, sofa::linearalgebra::FullVector<SReal>>;

    const auto matrixAccess =
        [](CRSLinearSystem& self) -> EigenDenseMatrixMap<SReal>
        {
            sofa::linearalgebra::FullMatrix<SReal>* matrix = self.getSystemMatrix();
            const auto row = matrix ? matrix->rows() : 0;
            const auto col = matrix ? matrix->cols() : 0;
            return EigenDenseMatrixMap<SReal>(matrix ? matrix->ptr() : nullptr, row, col);
        };
    c.def("A", matrixAccess, sofapython3::doc::linearsystem::linearSystem_FullMatrix_A);
    c.def("get_system_matrix", matrixAccess, sofapython3::doc::linearsystem::linearSystem_FullMatrix_get_system_matrix);
}

void moduleAddLinearSystem(py::module &m)
{
    bindLinearSystems<sofa::linearalgebra::FullMatrix<SReal>, sofa::linearalgebra::FullVector<SReal> >(m);
    bindLinearSystems<sofa::linearalgebra::SparseMatrix<SReal>, sofa::linearalgebra::FullVector<SReal> >(m);
    bindLinearSystems<sofa::linearalgebra::CompressedRowSparseMatrix<SReal>, sofa::linearalgebra::FullVector<SReal> >(m);
    bindLinearSystems<sofa::linearalgebra::CompressedRowSparseMatrix<sofa::type::Mat<2,2,SReal> >, sofa::linearalgebra::FullVector<SReal> >(m);
    bindLinearSystems<sofa::linearalgebra::CompressedRowSparseMatrix<sofa::type::Mat<3,3,SReal> >, sofa::linearalgebra::FullVector<SReal> >(m);
    bindLinearSystems<sofa::linearalgebra::CompressedRowSparseMatrix<sofa::type::Mat<4,4,SReal> >, sofa::linearalgebra::FullVector<SReal> >(m);
    bindLinearSystems<sofa::linearalgebra::CompressedRowSparseMatrix<sofa::type::Mat<6,6,SReal> >, sofa::linearalgebra::FullVector<SReal> >(m);
    bindLinearSystems<sofa::linearalgebra::CompressedRowSparseMatrix<sofa::type::Mat<8,8,SReal> >, sofa::linearalgebra::FullVector<SReal> >(m);
    bindLinearSystems<sofa::linearalgebra::DiagonalMatrix<SReal>, sofa::linearalgebra::FullVector<SReal> >(m);
    bindLinearSystems<sofa::linearalgebra::BlockDiagonalMatrix<3,SReal>, sofa::linearalgebra::FullVector<SReal> >(m);
    bindLinearSystems<sofa::linearalgebra::RotationMatrix<SReal>, sofa::linearalgebra::FullVector<SReal> >(m);

    bindLinearSystems<sofa::linearalgebra::BTDMatrix<6, SReal>, sofa::linearalgebra::BlockVector<6, SReal> >(m);
}

}

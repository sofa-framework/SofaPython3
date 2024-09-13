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
#include <SofaPython3/SofaLinearSystem/Binding_LinearSystem.h>
#include <SofaPython3/SofaLinearSystem/Binding_LinearSystem_doc.h>
#include <pybind11/pybind11.h>

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/PythonFactory.h>

#include <sofa/component/linearsystem/TypedMatrixLinearSystem.h>
#include <pybind11/eigen.h>
#include <sofa/linearalgebra/CompressedRowSparseMatrix.h>


namespace py { using namespace pybind11; }

namespace sofapython3 {

template<class Real>
using EigenSparseMatrix = Eigen::SparseMatrix<Real, Eigen::RowMajor>;

template<class Real>
using EigenMatrixMap = Eigen::Map<Eigen::SparseMatrix<Real, Eigen::RowMajor> >;

template<class Real>
using Vector = Eigen::Matrix<Real,Eigen::Dynamic, 1>;

template<class Real>
using EigenVectorMap = Eigen::Map<Vector<Real>>;

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

template<class TBlock>
void bindLinearSystems(py::module &m)
{
    using CRS = sofa::linearalgebra::CompressedRowSparseMatrix<TBlock>;
    using Real = typename CRS::Real;
    using CRSLinearSystem = sofa::component::linearsystem::TypedMatrixLinearSystem<CRS, sofa::linearalgebra::FullVector<Real> >;

    const std::string typeName = CRSLinearSystem::GetClass()->className + CRSLinearSystem::GetCustomTemplateName();

    py::class_<CRSLinearSystem,
               sofa::core::objectmodel::BaseObject,
               sofapython3::py_shared_ptr<CRSLinearSystem> > c(m, typeName.c_str(), sofapython3::doc::linearsystem::linearSystemClass);

    c.def("A", [](CRSLinearSystem& self) -> EigenSparseMatrix<Real>
    {
        if (CRS* matrix = self.getSystemMatrix())
        {
            if (matrix->colsValue.empty()) //null matrix: contains no entries
            {
                return EigenSparseMatrix<Real>{matrix->rows(), matrix->cols()};
            }
            return toEigen(*matrix);
        }
        return {};
    }, sofapython3::doc::linearsystem::linearSystem_A);

    c.def("b", [](CRSLinearSystem& self) -> Vector<Real>
    {
        if (auto* vector = self.getRHSVector())
        {
            return EigenVectorMap<Real>(vector->ptr(), vector->size());
        }
        return {};
    }, sofapython3::doc::linearsystem::linearSystem_b);

    c.def("x", [](CRSLinearSystem& self) -> Vector<Real>
    {
        if (auto* vector = self.getSolutionVector())
        {
            return EigenVectorMap<Real>(vector->ptr(), vector->size());
        }
        return {};
    }, sofapython3::doc::linearsystem::linearSystem_x);


    /// register the binding in the downcasting subsystem
    PythonFactory::registerType<CRSLinearSystem>([](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<CRSLinearSystem*>(object));
    });
}

void moduleAddLinearSystem(py::module &m)
{
    bindLinearSystems<SReal>(m);
    bindLinearSystems<sofa::type::Mat<3, 3, SReal> >(m);
}

}

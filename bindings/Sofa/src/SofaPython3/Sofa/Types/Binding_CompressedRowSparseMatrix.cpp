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
#include <SofaPython3/Sofa/Types/Binding_CompressedRowSparseMatrix.h>

#include <sofa/core/objectmodel/BaseData.h>
#include <sofa/core/objectmodel/Data.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/linearalgebra/CompressedRowSparseMatrixConstraint.h>
#include <pybind11/eigen.h>
#include <sofa/linearalgebra/CompressedRowSparseMatrixConstraintEigenUtils.h>
#include <sofa/type/trait/is_specialization_of.h>
#include <SofaPython3/PythonFactory.h>


namespace py { using namespace pybind11; }

namespace sofapython3
{

template<class TBlock>
typename sofa::linearalgebra::CompressedRowSparseMatrixToEigenSparseVec<TBlock>::EigenSparseMatrix
toEigen(const sofa::linearalgebra::CompressedRowSparseMatrixConstraint<TBlock>& matrix)
{
    static sofa::linearalgebra::CompressedRowSparseMatrixToEigenSparseVec<TBlock> convert;
    return convert(matrix);
}

template<class MatrixDeriv>
void bindCompressedRowSparseMatrixConstraint(pybind11::module& m)
{
    static_assert(sofa::type::trait::is_specialization_of_v<MatrixDeriv, sofa::linearalgebra::CompressedRowSparseMatrixConstraint>,
                  "Template argument must be a specialization of CompressedRowSparseMatrixConstraint");

    py::class_<sofa::Data<MatrixDeriv>, sofa::core::objectmodel::BaseData,
            std::unique_ptr<sofa::Data<MatrixDeriv>, pybind11::nodelete> > crsmc(m, MatrixDeriv::Name() );

    crsmc.def_property_readonly("value", [](sofa::Data<MatrixDeriv>& self)
    {
        sofa::helper::ReadAccessor accessor(self);
        return toEigen(accessor.ref());
    });

    PythonFactory::registerType(MatrixDeriv::Name(), [](sofa::core::BaseData* data) -> py::object {
        auto matrix = reinterpret_cast<sofa::Data<MatrixDeriv>*>(data);
        return py::cast(matrix);
    });
}

/**
 * Given a template parameter pack, create the bindings of all the templates in
 * the pack, making sure that there are no duplicate types.
 */
template <typename T, typename... Args>
void bindAllCompressedRowSparseMatrixConstraintTypes(pybind11::module& m)
{
    static_assert(!(std::is_same_v<T, Args> || ...), "Error: Duplicate types found!");

    bindCompressedRowSparseMatrixConstraint<T>(m);
    if constexpr (sizeof...(Args) > 0)
    {
        bindAllCompressedRowSparseMatrixConstraintTypes<Args...>(m);
    }
}

void moduleAddCompressedRowSparseMatrix(pybind11::module& m)
{
    using namespace sofa::defaulttype;

    bindAllCompressedRowSparseMatrixConstraintTypes<
        Vec1Types::MatrixDeriv,
        Vec2Types::MatrixDeriv,
        Vec3Types::MatrixDeriv,
        Vec6Types::MatrixDeriv,
        Rigid2Types::MatrixDeriv,
        Rigid3Types::MatrixDeriv
    >(m);
}

}

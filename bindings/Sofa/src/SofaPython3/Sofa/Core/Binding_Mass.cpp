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
#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_Mass.h>
#include <SofaPython3/Sofa/Core/Binding_Mass_doc.h>
#include <SofaPython3/PythonFactory.h>

#include <sofa/core/behavior/MechanicalState.h>
#include <sofa/core/behavior/Mass.h>
#include <sofa/core/MechanicalParams.h>
#include <sofa/core/behavior/DefaultMultiMatrixAccessor.h>
#include <sofa/linearalgebra/CompressedRowSparseMatrix.h>

#include <pybind11/eigen.h>

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

namespace sofapython3
{

using sofa::core::behavior::Mass;

template<class TDOFType>
void declare_mass(py::module &m) {
    const std::string pyclass_name = std::string("Mass") + TDOFType::Name();
    py::class_<Mass<TDOFType>, sofa::core::behavior::ForceField<TDOFType>, py_shared_ptr<Mass<TDOFType>>> f(m, pyclass_name.c_str(), sofapython3::doc::mass::massClass);

    using Real = typename TDOFType::Real;
    using EigenSparseMatrix = Eigen::SparseMatrix<typename TDOFType::Real, Eigen::RowMajor>;
    using EigenMatrixMap = Eigen::Map<EigenSparseMatrix>;

    f.def("assembleMMatrix", [](Mass<TDOFType>& self) -> EigenSparseMatrix
    {
        sofa::linearalgebra::CompressedRowSparseMatrix<Real> matrix;

        if (const auto* mstate = self.getMState())
        {
            const auto matrixSize = static_cast<sofa::linearalgebra::BaseMatrix::Index>(mstate->getMatrixSize());
            matrix.resize(matrixSize, matrixSize);

            sofa::core::behavior::DefaultMultiMatrixAccessor accessor;
            accessor.addMechanicalState(mstate);
            accessor.setGlobalMatrix(&matrix);

            auto mparams = *sofa::core::MechanicalParams::defaultInstance();
            mparams.setKFactor(0.).setMFactor(1.).setBFactor(0.);

            self.addMToMatrix(&mparams, &accessor);
        }
        matrix.compress();

        if (matrix.getColsValue().empty() || matrix.rowBegin.empty() || matrix.colsIndex.empty())
            return {};

        return EigenMatrixMap(matrix.rows(), matrix.cols(), matrix.getColsValue().size(),
                            (typename EigenMatrixMap::StorageIndex*)matrix.rowBegin.data(),
                            (typename EigenMatrixMap::StorageIndex*)matrix.colsIndex.data(),
                            matrix.colsValue.data());
    }, sofapython3::doc::mass::assembleMMatrix);

    PythonFactory::registerType<Mass<TDOFType>>([](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<Mass<TDOFType>*>(object));
    });
}


void moduleAddMass(py::module &m) {
    declare_mass<sofa::defaulttype::Vec3dTypes>(m);
    declare_mass<sofa::defaulttype::Vec2dTypes>(m);
    declare_mass<sofa::defaulttype::Vec1dTypes>(m);
    declare_mass<sofa::defaulttype::Vec6dTypes>(m);
    declare_mass<sofa::defaulttype::Rigid3dTypes>(m);
    declare_mass<sofa::defaulttype::Rigid2dTypes>(m);
}

}  // namespace sofapython3

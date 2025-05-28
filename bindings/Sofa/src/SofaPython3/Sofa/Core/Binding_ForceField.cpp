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
#include <pybind11/numpy.h>
#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_ForceField.h>
#include <SofaPython3/Sofa/Core/Binding_ForceField_doc.h>
#include <SofaPython3/PythonFactory.h>

#include <sofa/core/behavior/MechanicalState.h>
#include <sofa/core/behavior/ForceField.h>
#include <sofa/core/MechanicalParams.h>
#include <sofa/core/behavior/DefaultMultiMatrixAccessor.h>
#include <sofa/linearalgebra/CompressedRowSparseMatrix.h>

#include <pybind11/eigen.h>

#include <SofaPython3/PythonEnvironment.h>
using sofapython3::PythonEnvironment;

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }
/// To bring in the `_a` literal
using namespace pybind11::literals;

namespace sofapython3
{
    using sofa::core::objectmodel::BaseObject;
    using sofa::core::objectmodel::ComponentState;
    using sofa::core::behavior::MechanicalState;
    using sofa::core::MechanicalParams;
    using sofa::core::behavior::MultiMatrixAccessor;
    using sofa::core::behavior::ForceField;
    using sofa::defaulttype::Vec3dTypes;
    using sofa::defaulttype::Vec2dTypes;
    using sofa::defaulttype::Vec1dTypes;
    using sofa::defaulttype::Vec6dTypes;
    using sofa::defaulttype::Rigid3dTypes;
    using sofa::defaulttype::Rigid2dTypes;

    template<class TDOFType>
    ForceField_Trampoline<TDOFType>::ForceField_Trampoline() = default;

    template<class TDOFType>
    ForceField_Trampoline<TDOFType>::~ForceField_Trampoline() = default;

    template<class TDOFType>
    std::string ForceField_Trampoline<TDOFType>::getClassName() const
    {
        PythonEnvironment::gil acquire {"getClassName"};

        // Get the actual class name from python.
        return py::str(py::cast(this).get_type().attr("__name__"));
    }

    template<class TDOFType>
    void ForceField_Trampoline<TDOFType>::init()
    {
        ForceField<TDOFType>::init();

        PythonEnvironment::gil acquire;

        if (!mstate.get())
            mstate.set(dynamic_cast< MechanicalState<DataTypes>* >(getContext()->getMechanicalState()));

        if(!mstate.get())
            throw py::type_error("Missing mechanical state.");

        PYBIND11_OVERLOAD(void, ForceField<TDOFType>, init,);
    }

    template<class TDOFType>
    void ForceField_Trampoline<TDOFType>::addForce(const MechanicalParams* mparams,  DataVecDeriv& f, const DataVecCoord& x, const DataVecDeriv& v)
    {
        PythonEnvironment::gil acquire;

        // pass bFactor, kFactor, energy
        py::dict mp = py::dict("time"_a=getContext()->getTime(),
                               "mFactor"_a=mparams->mFactor(),
                               "bFactor"_a=mparams->bFactor(),
                               "kFactor"_a=mparams->kFactor(),
                               "isImplicit"_a=mparams->implicit(),
                               "energy"_a=mparams->energy());
        PYBIND11_OVERLOAD_PURE(void, ForceField<TDOFType>, addForce, mp, PythonFactory::toPython(&f), PythonFactory::toPython(&x), PythonFactory::toPython(&v));
    }

    template<class TDOFType>
    void ForceField_Trampoline<TDOFType>::addDForce(const MechanicalParams* mparams, DataVecDeriv& df, const DataVecDeriv& dx )
    {
        PythonEnvironment::gil acquire;

        // pass bFactor, kFactor, energy
        py::dict mp = py::dict("time"_a=getContext()->getTime(),
                               "nFactor"_a=mparams->mFactor(),
                               "bFactor"_a=mparams->bFactor(),
                               "kFactor"_a=mparams->kFactor(),
                               "isImplicit"_a=mparams->implicit()
                               );
        PYBIND11_OVERLOAD_PURE(void, ForceField<TDOFType>, addDForce, mp, PythonFactory::toPython(&df), PythonFactory::toPython(&dx));
    }


    template<class TDOFType>
    py::object ForceField_Trampoline<TDOFType>::_addKToMatrix(const MechanicalParams* mparams, int nIndices, int nDofs)
    {
        PythonEnvironment::gil acquire;

        py::dict mp = py::dict("time"_a=getContext()->getTime(),
                               "mFactor"_a=mparams->mFactor(),
                               "bFactor"_a=mparams->bFactor(),
                               "kFactor"_a=mparams->kFactor(),
                               "isImplicit"_a=mparams->implicit()
                );

        PYBIND11_OVERLOAD_PURE(py::object, ForceField<TDOFType>, addKToMatrix, mp, nIndices, nDofs);
    }

    template<class TDOFType>
    void ForceField_Trampoline<TDOFType>::doAddKToMatrix(const MechanicalParams* mparams, const MultiMatrixAccessor* dfId)
    {
        MultiMatrixAccessor::MatrixRef mref = dfId->getMatrix(this->mstate);
        sofa::linearalgebra::BaseMatrix* mat = mref.matrix;

        size_t offset = mref.offset;
        // nNodes is the number of nodes (positions) of the object whose K matrix we're computing
        int nNodes = int(mparams->readX(mstate.get())->getValue().size());
        // nDofs is the number of degrees of freedom per-element of the object whose K matrix we're computing
        int nDofs = Coord::total_size;

        py::object ret = _addKToMatrix(mparams, nNodes, nDofs);

        // if ret is numpy array
        if(py::isinstance<py::array>(ret))
        {
            auto r = py::cast<py::array>(ret);
            if (r.ndim() == 3 && r.shape(2) == 1)
            {
                // read K as a plain 2D matrix
                auto kMatrix = r.unchecked<double, 3>();
                for (size_t x = 0 ; x < size_t(kMatrix.shape(0)) ; ++x)
                {
                    for (size_t y = 0 ; y < size_t(kMatrix.shape(1)) ; ++y)
                    {
                        mat->add(int(offset + x), int(offset + y), kMatrix(x,y, 0));
                    }
                }
            }
            else if (r.ndim() == 2 && r.shape(1) == 3)
            {
                // consider ret to be a list of tuples [(i,j,[val])]
                auto kMatrix = r.unchecked<double, 2>();
                for (auto x = 0 ; x < kMatrix.shape(0) ; ++x)
                {
                    mat->add(int(offset + size_t(kMatrix(x,0))), int(offset + size_t(kMatrix(x,1))), kMatrix(x,2));
                }
            }
            else
            {
                throw py::type_error("Can't read return value of AddKToMatrix. The method should return either a plain 2D matrix or a vector of tuples (i, j, val)");
            }
        }
    }


    template<class TDOFType>
    void declare_forcefield(py::module &m) {
        const std::string pyclass_name = std::string("ForceField") + TDOFType::Name();
        py::class_<ForceField<TDOFType>, BaseObject, ForceField_Trampoline<TDOFType>, py_shared_ptr<ForceField<TDOFType>>> f(m, pyclass_name.c_str(), py::dynamic_attr(), py::multiple_inheritance(), sofapython3::doc::forceField::forceFieldClass);

        f.def(py::init([](py::args &args, py::kwargs &kwargs) {
            auto ff = sofa::core::sptr<ForceField_Trampoline<TDOFType>> (new ForceField_Trampoline<TDOFType>());

            ff->f_listening.setValue(true);

            if (args.size() == 1) ff->setName(py::cast<std::string>(args[0]));

            py::object cc = py::cast(ff);
            for (auto kv : kwargs) {
                std::string key = py::cast<std::string>(kv.first);
                py::object value = py::reinterpret_borrow<py::object>(kv.second);
                if (key == "name") {
                    if (args.size() != 0) {
                        throw py::type_error("The name is set twice as a "
                                            "named argument='" + py::cast<std::string>(value) + "' and as a"
                                                                                                "positional argument='" +
                                            py::cast<std::string>(args[0]) + "'.");
                    }
                }
                BindingBase::SetAttr(cc, key, value);
            }
            return ff;
        }));

        using Real = typename TDOFType::Real;
        using EigenSparseMatrix = Eigen::SparseMatrix<typename TDOFType::Real, Eigen::RowMajor>;
        using EigenMatrixMap = Eigen::Map<EigenSparseMatrix>;

        f.def("assembleKMatrix", [](ForceField<TDOFType>& self) -> EigenSparseMatrix
        {
            sofa::linearalgebra::CompressedRowSparseMatrix<Real> matrix;

            if (const auto* mstate = self.getMState())
            {
                const auto matrixSize = static_cast<sofa::linearalgebra::BaseMatrix::Index>(mstate->getMatrixSize());
                matrix.resize(matrixSize, matrixSize);

                sofa::core::behavior::DefaultMultiMatrixAccessor accessor;
                accessor.addMechanicalState(mstate);
                accessor.setGlobalMatrix(&matrix);

                auto mparams = *MechanicalParams::defaultInstance();
                mparams.setKFactor(1.).setMFactor(0.).setBFactor(0.);

                self.addKToMatrix(&mparams, &accessor);
            }
            matrix.compress();

            if (matrix.getColsValue().empty() || matrix.rowBegin.empty() || matrix.colsIndex.empty())
                return {};

            return EigenMatrixMap(matrix.rows(), matrix.cols(), matrix.getColsValue().size(),
                                (typename EigenMatrixMap::StorageIndex*)matrix.rowBegin.data(),
                                (typename EigenMatrixMap::StorageIndex*)matrix.colsIndex.data(),
                                matrix.colsValue.data());
        }, sofapython3::doc::forceField::assembleKMatrix);

        PythonFactory::registerType<ForceField<TDOFType>>([](sofa::core::objectmodel::Base* object)
        {
            return py::cast(dynamic_cast<ForceField<TDOFType>*>(object));
        });
    }


void moduleAddForceField(py::module &m) {
    declare_forcefield<Vec3dTypes>(m);
    declare_forcefield<Vec2dTypes>(m);
    declare_forcefield<Vec1dTypes>(m);
    declare_forcefield<Vec6dTypes>(m);
    declare_forcefield<Rigid3dTypes>(m);
    declare_forcefield<Rigid2dTypes>(m);
}

}  // namespace sofapython3

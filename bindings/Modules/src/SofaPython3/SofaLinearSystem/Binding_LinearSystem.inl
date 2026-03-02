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
#pragma once
#include <SofaPython3/SofaLinearSystem/Binding_LinearSystem.h>
#include <SofaPython3/SofaLinearSystem/Binding_LinearSystem_doc.h>
#include <sofa/component/linearsystem/TypedMatrixLinearSystem.h>
#include <pybind11/pybind11.h>

#include <SofaPython3/PythonFactory.h>
#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <sofa/linearalgebra/EigenSparseMatrix.h>

namespace py { using namespace pybind11; }

namespace sofapython3
{

template<class Real>
using Vector = Eigen::Matrix<Real,Eigen::Dynamic, 1>;

template<class Real>
using EigenVectorMap = Eigen::Map<Vector<Real>>;

template<class TVector>
Vector<typename TVector::Real>
getVector(TVector* vector)
{
    using Real = typename TVector::Real;
    if (vector)
    {
        return EigenVectorMap<Real>(vector->ptr(), vector->size());
    }
    return {};
}

template<class TMatrix, class TVector>
Vector<typename TVector::Real> getRHSVector(sofa::component::linearsystem::TypedMatrixLinearSystem<TMatrix, TVector>& linearSystem)
{
    return getVector(linearSystem.getRHSVector());
}

template<class TMatrix, class TVector>
Vector<typename TVector::Real> getSolutionVector(sofa::component::linearsystem::TypedMatrixLinearSystem<TMatrix, TVector>& linearSystem)
{
    return getVector(linearSystem.getSolutionVector());
}

template<class TMatrix, class TVector>
using LinearSystemClass =
    py::class_<sofa::component::linearsystem::TypedMatrixLinearSystem<TMatrix, TVector>,
           sofa::core::objectmodel::BaseObject,
           sofapython3::py_shared_ptr<sofa::component::linearsystem::TypedMatrixLinearSystem<TMatrix, TVector>> >;

template<class TMatrix, class TVector>
void bindMatrixAccess(LinearSystemClass<TMatrix, TVector>& c)
{}

template<class TMatrix, class TVector>
void bindLinearSystems(py::module &m)
{
    using LinearSystem = sofa::component::linearsystem::TypedMatrixLinearSystem<TMatrix, TVector>;

    const std::string typeName =
        LinearSystem::GetClass()->className
        + LinearSystem::GetCustomTemplateName();

    LinearSystemClass<TMatrix, TVector> c(m, typeName.c_str(), sofapython3::doc::linearsystem::linearSystemClass);

    c.def("b", getRHSVector<TMatrix, TVector>, sofapython3::doc::linearsystem::linearSystem_b);
    c.def("get_rhs_vector", getRHSVector<TMatrix, TVector>, sofapython3::doc::linearsystem::linearSystem_b);

    c.def("x", getSolutionVector<TMatrix, TVector>, sofapython3::doc::linearsystem::linearSystem_x);
    c.def("get_solution_vector", getSolutionVector<TMatrix, TVector>, sofapython3::doc::linearsystem::linearSystem_x);

    bindMatrixAccess<TMatrix, TVector>(c);

    /// register the binding in the downcasting subsystem
    PythonFactory::registerType<LinearSystem>([](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<LinearSystem*>(object));
    });
}

}

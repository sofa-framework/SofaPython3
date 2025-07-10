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
#include <SofaPython3/Sofa/Core/Binding_Mapping.h>
#include <SofaPython3/Sofa/Core/Binding_Mapping_doc.h>
#include <SofaPython3/PythonFactory.h>

#include <sofa/core/ConstraintParams.h>

#include <sofa/core/behavior/MechanicalState.h>
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
    using sofa::core::Mapping;
    using sofa::core::objectmodel::BaseObject;
    using sofa::core::objectmodel::ComponentState;
    using sofa::core::behavior::MechanicalState;
    using sofa::core::MechanicalParams;
    using sofa::core::behavior::MultiMatrixAccessor;
    using sofa::defaulttype::Vec3dTypes;
    using sofa::defaulttype::Vec2dTypes;
    using sofa::defaulttype::Vec1dTypes;
    using sofa::defaulttype::Vec6dTypes;
    using sofa::defaulttype::Rigid3dTypes;
    using sofa::defaulttype::Rigid2dTypes;

    template<class In, class Out>
    Mapping_Trampoline<In, Out>::Mapping_Trampoline() = default;

    template<class In, class Out>
    Mapping_Trampoline<In, Out>::~Mapping_Trampoline() = default;

    template<class In, class Out>
    std::string Mapping_Trampoline<In, Out>::getClassName() const
    {
        PythonEnvironment::gil acquire {"getClassName"};

        // Get the actual class name from python.
        return py::str(py::cast(this).get_type().attr("__name__"));
    }

    template<class In, class Out>
    void Mapping_Trampoline<In, Out>::init()
    {
        PythonEnvironment::gil acquire;

        Inherit1::init();
    }

    template<class In, class Out>
    void Mapping_Trampoline<In, Out>::apply( const MechanicalParams* mparams, OutDataVecCoord& out, const InDataVecCoord& in){
        PythonEnvironment::gil acquire;

        // pass bFactor, kFactor, energy
        py::dict mp = py::dict("time"_a=this->getContext()->getTime(),
                               "mFactor"_a=mparams->mFactor(),
                               "bFactor"_a=mparams->bFactor(),
                               "kFactor"_a=mparams->kFactor(),
                               "isImplicit"_a=mparams->implicit(),
                               "energy"_a=mparams->energy());

        PYBIND11_OVERLOAD_PURE(void, Inherit1, apply, mp,
                               PythonFactory::toPython(&out), PythonFactory::toPython(&in));
    }

    template<class In, class Out>
    void Mapping_Trampoline<In, Out>::applyJ( const MechanicalParams* mparams, OutDataVecDeriv& out, const InDataVecDeriv& in){
        PythonEnvironment::gil acquire;

        // pass bFactor, kFactor, energy
        py::dict mp = py::dict("time"_a=getContext()->getTime(),
                               "mFactor"_a=mparams->mFactor(),
                               "bFactor"_a=mparams->bFactor(),
                               "kFactor"_a=mparams->kFactor(),
                               "isImplicit"_a=mparams->implicit(),
                               "energy"_a=mparams->energy());

        PYBIND11_OVERLOAD_PURE(void, Inherit1, applyJ, mp,
                               PythonFactory::toPython(&out), PythonFactory::toPython(&in));
    }

    template<class In, class Out>
    void Mapping_Trampoline<In, Out>::applyJT( const MechanicalParams* mparams, InDataVecDeriv& out, const OutDataVecDeriv& in){
        PythonEnvironment::gil acquire;

        py::dict mp = py::dict("time"_a=getContext()->getTime(),
                               "mFactor"_a=mparams->mFactor(),
                               "bFactor"_a=mparams->bFactor(),
                               "kFactor"_a=mparams->kFactor(),
                               "isImplicit"_a=mparams->implicit(),
                               "energy"_a=mparams->energy());

        PYBIND11_OVERLOAD_PURE(void, Inherit1, applyJT, mp,
                               PythonFactory::toPython(&out), PythonFactory::toPython(&in));
    }

    template<class In, class Out>
    void Mapping_Trampoline<In, Out>::applyJT( const ConstraintParams* cparams,
                                                         InDataMatrixDeriv& out, const OutDataMatrixDeriv& in)
    {
        PythonEnvironment::gil acquire;

        py::dict mp = py::dict("time"_a=getContext()->getTime());

        PYBIND11_OVERLOAD_PURE(void, Inherit1, applyConstrainsJT, mp,
                               PythonFactory::toPython(&out), PythonFactory::toPython(&in));
    }

    template<class In, class Out>
    void declareMapping(py::module &m) {
        const std::string pyclass_name = std::string("Mapping_") + In::Name()+ "_" + Out::Name();

        py::class_<Mapping<In, Out>, BaseObject, Mapping_Trampoline<In, Out>,
                   py_shared_ptr<Mapping<In, Out>>> f(m, pyclass_name.c_str(), py::dynamic_attr(), py::multiple_inheritance(),
                                                      sofapython3::doc::mapping::mappingClass);

        f.def(py::init([](py::args &args, py::kwargs &kwargs) {
            auto ff = sofa::core::sptr<Mapping_Trampoline<In, Out>> (new Mapping_Trampoline<In, Out>());

            ff->f_listening.setValue(true);

            if (args.size() == 1)
                ff->setName(py::cast<std::string>(args[0]));

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
    }

    void moduleAddMapping(py::module &m) {
        declareMapping<Rigid3dTypes, Vec3dTypes>(m);
        declareMapping<Vec3dTypes, Vec3dTypes>(m);
        declareMapping<Vec3dTypes, Vec1dTypes>(m);
    }

}  // namespace sofapython3

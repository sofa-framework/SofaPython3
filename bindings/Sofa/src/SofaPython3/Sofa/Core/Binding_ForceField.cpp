#include <pybind11/pybind11.h>
#include <pybind11/detail/init.h>
#include "Binding_BaseObject.h"
#include "Binding_ForceField.h"
#include <SofaPython3/DataHelper.h>
#include <SofaPython3/PythonFactory.h>

#include <sofa/core/behavior/MechanicalState.h>
#include <sofa/core/behavior/ForceField.h>
#include <sofa/simulation/Node.h>
#include <sofa/core/MechanicalParams.h>


PYBIND11_DECLARE_HOLDER_TYPE(TForceField,
                             sofapython3::py_shared_ptr<TForceField>, true)


namespace sofapython3
{
    using sofa::core::objectmodel::ComponentState;
    using sofa::core::behavior::MechanicalState;
    using sofa::core::MechanicalParams;
    using sofa::core::behavior::MultiMatrixAccessor;
    using sofa::core::behavior::ForceField;
    using sofa::defaulttype::Vec3dTypes;



    ForceField_Trampoline::ForceField_Trampoline() = default;
    ForceField_Trampoline::~ForceField_Trampoline() = default;

    void ForceField_Trampoline::init()
    {
        ForceField<Vec3dTypes>::init();

        if (!mstate.get())
            mstate.set(dynamic_cast< MechanicalState<DataTypes>* >(getContext()->getMechanicalState()));

        if(!mstate.get())
            throw py::type_error("Missing mechanical state.");

        PYBIND11_OVERLOAD(void, ForceField, init,);
    }

    // pass bFactor, kFactor, readX, readV, readF, energy
    void ForceField_Trampoline::addForce(const MechanicalParams* mparams,  DataVecDeriv& f, const DataVecCoord& x, const DataVecDeriv& v)
    {
        // pass bFactor, kFactor, energy
        py::dict mp = py::dict("mFactor"_a=mparams->mFactor(),
                               "bFactor"_a=mparams->bFactor(),
                               "kFactor"_a=mparams->kFactor(),
                               "energy"_a=mparams->energy());
        PYBIND11_OVERLOAD_PURE(void, ForceField, addForce, mp, PythonFactory::toPython(&f), PythonFactory::toPython(&x), PythonFactory::toPython(&v));
    }


    // pass mFactor, kFactor
    void ForceField_Trampoline::addDForce(const MechanicalParams* mparams, DataVecDeriv& df, const DataVecDeriv& dx )
    {
        // pass bFactor, kFactor, energy
        py::dict mp = py::dict("nFactor"_a=mparams->mFactor(),
                               "bFactor"_a=mparams->bFactor(),
                               "kFactor"_a=mparams->kFactor());
        PYBIND11_OVERLOAD_PURE(void, ForceField, addDForce, mp, PythonFactory::toPython(&df), PythonFactory::toPython(&dx));
    }

    py::object ForceField_Trampoline::_addKToMatrix(const MechanicalParams* mparams, int nIndices, int nDofs)
    {
        py::dict mp = py::dict("mFactor"_a=mparams->mFactor(),
                               "bFactor"_a=mparams->bFactor(),
                               "kFactor"_a=mparams->kFactor());

        PYBIND11_OVERLOAD_PURE(py::object, ForceField, addKToMatrix, mp, nIndices, nDofs);
    }


    // pass k/b/mFactor
    void ForceField_Trampoline::addKToMatrix(const MechanicalParams* mparams, const MultiMatrixAccessor* dfId)
    {
        MultiMatrixAccessor::MatrixRef mref = dfId->getMatrix(this->mstate);
        sofa::defaulttype::BaseMatrix* mat = mref.matrix;

        size_t offset = mref.offset;
        // nNodes is the number of nodes (positions) of the object whose K matrix we're computing
        int nNodes = int(mparams->readX(mstate)->getValue().size());
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

    std::string ForceField_Trampoline::getClassName() const
    {
        return pyobject->ob_type->tp_name;
    }



    void moduleAddForceField(py::module &m)
    {
        py::class_<ForceField<Vec3dTypes>,
                BaseObject, ForceField_Trampoline,
                py_shared_ptr<ForceField<Vec3dTypes>>> f(m, "ForceField",
                                                         py::dynamic_attr(),
                                                         py::multiple_inheritance());

        f.def(py::init([](py::args& args, py::kwargs& kwargs)
        {
                  auto c = new ForceField_Trampoline();
                  c->f_listening.setValue(true);

                  if(args.size() != 0)
                  {
                      if(args.size()==1) c->setName(py::cast<std::string>(args[0]));
                      else throw py::type_error("Only one un-named arguments can be provided.");
                  }

                  py::object cc = py::cast(c);
                  for(auto kv : kwargs)
                  {
                      std::string key = py::cast<std::string>(kv.first);
                      py::object value = py::reinterpret_borrow<py::object>(kv.second);
                      if( key == "name")
                      {
                          if( args.size() != 0 )
                          {
                              throw py::type_error("The name is setted twice as a "
                              "named argument='"+py::cast<std::string>(value)+"' and as a"
                              "positional argument='"+py::cast<std::string>(args[0])+"'.");
                          }
                      }
                      BindingBase::SetAttr(cc, key, value);
                  }
                  return c;
              }));
    }

}  // namespace sofapython3

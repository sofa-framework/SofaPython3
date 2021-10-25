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

#include <SofaPython3/Sofa/Core/Binding_OdeSolver.h>
#include <SofaPython3/Sofa/Core/Binding_OdeSolver_doc.h>
#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <sofa/core/behavior/OdeSolver.h>
#include <sofa/core/ExecParams.h>

#include <SofaPython3/PythonFactory.h>
#include <SofaPython3/PythonEnvironment.h>

namespace py { using namespace pybind11; }

namespace sofapython3 {
using namespace sofa::core::behavior;

class PyOdeSolver : public OdeSolver {
public:
    SOFA_CLASS(PyOdeSolver, OdeSolver);
    void solve(const sofa::core::ExecParams* params, SReal dt, sofa::core::MultiVecCoordId xResult, sofa::core::MultiVecDerivId vResult) final {
        PythonEnvironment::gil acquire;
        PYBIND11_OVERLOAD_PURE(void, OdeSolver, solve, params, dt, xResult, vResult);
    }
};

void moduleAddOdeSolver(py::module& m) {
    py::class_<OdeSolver, sofa::core::objectmodel::BaseObject, PyOdeSolver, py_shared_ptr<OdeSolver>> (m, "OdeSolver", doc::odesolver::OdeSolverClass)
    .def(py::init([](py::args & /*args*/, py::kwargs & kwargs) {
        auto o = sofa::core::sptr<PyOdeSolver> (new PyOdeSolver());
        for (auto kv : kwargs) {
            auto key = py::cast<std::string>(kv.first);
            auto value = py::reinterpret_borrow<py::object>(kv.second);
            BindingBase::SetAttr(*o.get(), key, value);
        }
        return o;
    }))
    ;

    /// register the BaseState binding in the downcasting subsystem
    PythonFactory::registerType<OdeSolver>([](sofa::core::objectmodel::Base* object)
   {
       return py::cast(dynamic_cast<OdeSolver*>(object));
   });
}

} // namespace sofapython3

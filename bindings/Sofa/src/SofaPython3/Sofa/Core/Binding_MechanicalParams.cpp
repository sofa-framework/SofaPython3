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

#include <SofaPython3/Sofa/Core/Binding_MechanicalParams.h>
#include <SofaPython3/Sofa/Core/Binding_MechanicalParams_doc.h>
#include <sofa/core/MechanicalParams.h>

namespace py { using namespace pybind11; }

namespace sofapython3 {

void moduleAddMechanicalParams(py::module& m) {
    using namespace sofa::core;
    py::class_<MechanicalParams, ExecParams> c (m, "MechanicalParams", doc::mechanicalparams::MechanicalParamsClass);
    c.def(py::init());

    // Dt
    c.def_property("dt", &MechanicalParams::dt, &MechanicalParams::setDt);
    c.def("setDt", &MechanicalParams::setDt, py::arg("dt"));

    // Implicit
    c.def_property("implicit", &MechanicalParams::implicit, &MechanicalParams::setImplicit, "Specify if the time integration scheme is implicit.");
    c.def("setImplicit", &MechanicalParams::setImplicit, py::arg("v"), "Specify if the time integration scheme is implicit.");

    // Mass factor
    c.def_property("mFactor", &MechanicalParams::mFactor, &MechanicalParams::setMFactor, "Set Mass matrix contributions factor (for implicit schemes).");
    c.def("setMFactor", &MechanicalParams::setMFactor, py::arg("m"), "Set Mass matrix contributions factor (for implicit schemes).");

    // Damping factor
    c.def_property("bFactor", &MechanicalParams::bFactor, &MechanicalParams::setBFactor, "Set Damping matrix contributions factor (for implicit schemes).");
    c.def("setBFactor", &MechanicalParams::setBFactor, py::arg("b"), "Set Damping matrix contributions factor (for implicit schemes).");

    // Stiffness factor
    c.def_property("kFactor", &MechanicalParams::kFactor, &MechanicalParams::setKFactor, "Set Stiffness matrix contributions factor (for implicit schemes).");
    c.def("setKFactor", &MechanicalParams::setKFactor, py::arg("k"), "Set Stiffness matrix contributions factor (for implicit schemes).");

    // Symmetric
    c.def_property("symmetricMatrix", &MechanicalParams::symmetricMatrix, &MechanicalParams::setSymmetricMatrix, "Set the symmetric matrix flag (for implicit schemes), for solvers specialized on symmetric matrices");
    c.def("setSymmetricMatrix", &MechanicalParams::setSymmetricMatrix, "Set the symmetric matrix flag (for implicit schemes), for solvers specialized on symmetric matrices");

    // Energy
    c.def_property("energy", &MechanicalParams::energy, &MechanicalParams::setEnergy, "Specify if the potential and kinematic energies should be computed.");
    c.def("setEnergy", &MechanicalParams::setEnergy, "Specify if the potential and kinematic energies should be computed.");
}

} // namespace sofapython3

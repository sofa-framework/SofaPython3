#pragma once

#include <SofaPython3/Sofa/Core/Binding_BaseObject.h>

#include <sofa/simulation/Simulation.h>
using sofa::simulation::Simulation;

template class pybind11::class_<Simulation, Simulation::SPtr>;

namespace sofapython3
{

//void moduleAddSimulation(py::module &m);
void moduleAddSimulation(py::module &m);

} ///sofapython3


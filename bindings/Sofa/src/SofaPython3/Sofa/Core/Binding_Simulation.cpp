#include "Binding_Simulation.h"

#include <sofa/simulation/Simulation.h>
using sofa::simulation::Simulation;

#include <SofaSimulationGraph/DAGSimulation.h>
using sofa::simulation::graph::DAGSimulation;

using sofa::simulation::Node;
using namespace pybind11::literals;

namespace sofapython3
{

class PySimulation : public DAGSimulation
{
public:
    typedef sofa::core::sptr<PySimulation> SPtr;
    Node::SPtr  m_root;
    Node::SPtr getRootNode(){ return m_root; }
    void setRootNode(Node::SPtr root) { m_root = root; }
};

void moduleAddSimulation(py::module &m)
{
//  m.def("createSimulationFromFile", [](const std::string& filename) -> py::object
//  {
//      /// set the Simulation, replacing the existing one (which is automatically deleted)
//      PySimulation::SPtr newSimulation = new PySimulation();

//      SceneLoader* loader=SceneLoaderFactory::getInstance()->getEntryFileName(filename);
//      if(!loader)
//      {
//          return py::none();
//      }

//      Node::SPtr root = loader->load(filename.c_str());
//      newSimulation->setInstanciationSourceFileName(filename);
//      newSimulation->setRootNode(root);
//      return py::cast(newSimulation);
//  });

//  m.def("createSimulationFromNode", [](sofa::simulation::Node* root) -> py::object
//  {
//      /// set the Simulation, replacing the existing one (which is automatically deleted)
//      PySimulation::SPtr newSimulation = new PySimulation();

//      /// Get the current python file.
//      newSimulation->setInstanciationSourceFileName("");
//      newSimulation->setRootNode(root);
//      return py::cast(newSimulation);
//  });

  py::class_<PySimulation, PySimulation::SPtr> s(m, "Simulation");
  s.def(py::init([](){ return new PySimulation(); }));

  s.def("init", [](PySimulation& self)
  {
      self.init(self.getRootNode().get());
  });

  s.def("animate", [](PySimulation& self, double dt)
  {
      self.animate(self.getRootNode().get(), dt);
  });

  s.def("dump", [](PySimulation& self)
  {
      self.print(self.getRootNode().get());
  });

  s.def("load", [](PySimulation& self, const std::string& filename)
  {
      self.setRootNode(self.load(filename.c_str()));
      return self.getRootNode();
  });
  s.def("load", [](PySimulation& self, const std::string& filename,bool reload)
  {
      self.setRootNode(self.load(filename.c_str(),reload));
      return self.getRootNode();
  });

//  s.def("print", &Simulation::print, "node"_a);
//  s.def("init", &Simulation::init, "node"_a);
//  s.def("animate", &Simulation::animate, "node"_a, "dt"_a = 0.0);
//  s.def("reset", &Simulation::reset, "node"_a);
    s.def("unload", &Simulation::unload, "node"_a);
}

void moduleAddRuntime(py::module &m)
{
  py::module singleRuntime = m.def_submodule("SingleSimulation");
  singleRuntime.def("setSimulation", [](Simulation *s){ sofa::simulation::setSimulation(s); });
  singleRuntime.def("getSimulation", [](){ return sofa::simulation::getSimulation(); });

  singleRuntime.def("print", [](Node* n){ sofa::simulation::getSimulation()->print(n); });
  singleRuntime.def("animate", [](Node* n, SReal dt=0.0){ sofa::simulation::getSimulation()->animate(n, dt); });
  singleRuntime.def("init", [](Node* n){ sofa::simulation::getSimulation()->init(n); });
  singleRuntime.def("reset", [](Node* n){ sofa::simulation::getSimulation()->reset(n); });
  singleRuntime.def("load", [](const std::string name){ return sofa::simulation::getSimulation()->load(name.c_str());});
  singleRuntime.def("unload", [](Node* n){ sofa::simulation::getSimulation()->unload(n); });

}


} /// namespace sofapython3

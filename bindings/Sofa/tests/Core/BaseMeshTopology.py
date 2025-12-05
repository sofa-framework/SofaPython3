import unittest
import gc
import Sofa.Core
import Sofa.Simulation
import SofaRuntime

class Test(unittest.TestCase):
    def test_regular_grid_hexas(self):
        SofaRuntime.importPlugin("Sofa.Component.Topology.Container.Grid")

        root = Sofa.Core.Node("rootNode")
        topology = root.addObject('RegularGridTopology', n=[3, 3, 3], min=[0,0,0], max=[1,1,1])

        self.assertTrue(topology is not None)

        with self.subTest(msg='getNbPoints'):
            self.assertEqual(topology.getNbPoints(), 27)

        with self.subTest(msg='getNbLines'):
            self.assertEqual(topology.getNbLines(), 90)
        with self.subTest(msg='getNbEdges'):
            self.assertEqual(topology.getNbEdges(), 90)

        with self.subTest(msg='getNbTriangles'):
            self.assertEqual(topology.getNbTriangles(), 72)
        with self.subTest(msg='getNbTetrahedra'):
            self.assertEqual(topology.getNbTetrahedra(), 0)
        with self.subTest(msg='getNbHexahedra'):
            self.assertEqual(topology.getNbHexahedra(), 8)
        with self.subTest(msg='getNbQuads'):
            self.assertEqual(topology.getNbQuads(), 36)
        with self.subTest(msg='getNbTetras'):
            self.assertEqual(topology.getNbTetras(), 0)

        with self.subTest(msg='getEdge'):
            edge_0 = topology.getEdge(0);
            self.assertEqual(len(edge_0), 2)
            self.assertEqual(edge_0[0], 1)
            self.assertEqual(edge_0[1], 4)

        with self.subTest(msg='getHexahedron'):
            hexa_0 = topology.getHexahedron(0);
            self.assertEqual(len(hexa_0), 8)
            self.assertEqual(hexa_0[0], 0)
            self.assertEqual(hexa_0[1], 1)
            self.assertEqual(hexa_0[2], 4)
            self.assertEqual(hexa_0[3], 3)
            self.assertEqual(hexa_0[4], 9)
            self.assertEqual(hexa_0[5], 10)
            self.assertEqual(hexa_0[6], 13)
            self.assertEqual(hexa_0[7], 12)

        with self.subTest(msg='getEdgesAroundVertex'):
            edges = topology.getEdgesAroundVertex(0)
            self.assertEqual(len(edges), 6)


    def test_regular_grid_tetra(self):
        SofaRuntime.importPlugin("Sofa.Component.Topology.Container.Grid")
        SofaRuntime.importPlugin("Sofa.Component.Topology.Container.Constant")
        SofaRuntime.importPlugin("Sofa.Component.Topology.Container.Dynamic")
        SofaRuntime.importPlugin("Sofa.Component.Topology.Mapping")

        root = Sofa.Core.Node("rootNode")
        grid = root.addObject('RegularGridTopology', n=[3, 3, 3], min=[0,0,0], max=[1,1,1])
        tetra_container = root.addObject('TetrahedronSetTopologyContainer')
        root.addObject('TetrahedronSetTopologyModifier')
        mapping = root.addObject('Hexa2TetraTopologicalMapping', input=grid.getLinkPath(), output=tetra_container.getLinkPath())
        topology = root.addObject('MeshTopology',
            position=tetra_container.edges.linkpath,
            edges=tetra_container.edges.linkpath,
            triangles=tetra_container.triangles.linkpath,
            tetrahedra=tetra_container.tetrahedra.linkpath
        )

        root.init()

        self.assertTrue(topology is not None)

        with self.subTest(msg='getNbPoints'):
            self.assertEqual(topology.getNbPoints(), 27)

        with self.subTest(msg='getNbLines'):
            self.assertEqual(topology.getNbLines(), 98)
        with self.subTest(msg='getNbEdges'):
            self.assertEqual(topology.getNbEdges(), 98)

        with self.subTest(msg='getNbTriangles'):
            self.assertEqual(topology.getNbTriangles(), 120)
        with self.subTest(msg='getNbTetrahedra'):
            self.assertEqual(topology.getNbTetrahedra(), 48)
        with self.subTest(msg='getNbHexahedra'):
            self.assertEqual(topology.getNbHexahedra(), 0)
        with self.subTest(msg='getNbQuads'):
            self.assertEqual(topology.getNbQuads(), 0)
        with self.subTest(msg='getNbTetras'):
            self.assertEqual(topology.getNbTetras(), 48)

        with self.subTest(msg='getEdge'):
            edge_0 = topology.getEdge(0);
            self.assertEqual(len(edge_0), 2)
            self.assertEqual(edge_0[0], 0)
            self.assertEqual(edge_0[1], 10)

        with self.subTest(msg='getTetrahedron'):
            tetra_0 = topology.getTetrahedron(0);
            self.assertEqual(len(tetra_0), 4)
            self.assertEqual(tetra_0[0], 0)
            self.assertEqual(tetra_0[1], 10)
            self.assertEqual(tetra_0[2], 1)
            self.assertEqual(tetra_0[3], 13)

        with self.subTest(msg='getTrianglesInTetrahedron'):
            triangles = topology.getTrianglesInTetrahedron(0);
            self.assertEqual(len(triangles), 4)

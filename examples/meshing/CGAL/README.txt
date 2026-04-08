This folder presents two simple modules using CGAL bindings allowing for easy tetrahedric mesh generation from either a polyhedron of its surface or a point cloud (using a convexe hull).

You can you this through the two mains (mesh_from_point_cloud_using_convex_hull.py and mesh_from_polyhedron.py) through command line (use -h for more information) or simply use the cgal_utils.py module if you want to build new scripts upon this work. The most important class being CGAL_Mesh_3_IO_Util heping to deal with the CGal data structure and use they results as numpy array. To see usage example, see mesh_from_polyhedron.py:CGAL_Mesh_from_polyhedron().

Those scripts are here to replace the components MeshGenerationFromPolyhedron and TriangularConvexHull3D from the CGALPLugin https://github.com/sofa-framework/CGALPlugin

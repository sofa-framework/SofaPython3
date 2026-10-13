project(Examples)


set(EXAMPLES_FILES

    ${CMAKE_CURRENT_SOURCE_DIR}/access_compliance_matrix.py
    ${CMAKE_CURRENT_SOURCE_DIR}/access_mass_matrix.py
    ${CMAKE_CURRENT_SOURCE_DIR}/access_matrix.py
    ${CMAKE_CURRENT_SOURCE_DIR}/access_stiffness_matrix.py
    ${CMAKE_CURRENT_SOURCE_DIR}/additional-examples/ControllerScene.py
    ${CMAKE_CURRENT_SOURCE_DIR}/advanced_timer.py
    ${CMAKE_CURRENT_SOURCE_DIR}/basic-addGUI.py
    ${CMAKE_CURRENT_SOURCE_DIR}/basic.py
    ${CMAKE_CURRENT_SOURCE_DIR}/CCDIntersection.py
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaUnitsExample.py
    ${CMAKE_CURRENT_SOURCE_DIR}/emptyController.py
    ${CMAKE_CURRENT_SOURCE_DIR}/emptyDataEngine.py
    ${CMAKE_CURRENT_SOURCE_DIR}/emptyForceField.py
    ${CMAKE_CURRENT_SOURCE_DIR}/example-forcefield.py
    ${CMAKE_CURRENT_SOURCE_DIR}/example-scriptcontroller.py
    ${CMAKE_CURRENT_SOURCE_DIR}/keyEvents.py
    ${CMAKE_CURRENT_SOURCE_DIR}/liver.py
    ${CMAKE_CURRENT_SOURCE_DIR}/liver-scriptcontroller.py
    ${CMAKE_CURRENT_SOURCE_DIR}/loadXMLfromPython.py
    ${CMAKE_CURRENT_SOURCE_DIR}/pointSetTopologyModifier.py
    ${CMAKE_CURRENT_SOURCE_DIR}/ReadTheDocs_Example.py
    ${CMAKE_CURRENT_SOURCE_DIR}/springForceField.py
    ${CMAKE_CURRENT_SOURCE_DIR}/jax/forcefield.py
    ${CMAKE_CURRENT_SOURCE_DIR}/RPYC/ExampleFileOnRemote.py 
    ${CMAKE_CURRENT_SOURCE_DIR}/RPYC/ExampleLocalCreateSceneMethod.py
    ${CMAKE_CURRENT_SOURCE_DIR}/RPYC/ExampleManipulatingTheNodeLocally.py 
    ${CMAKE_CURRENT_SOURCE_DIR}/RPYC/SOFAService.py
    ${CMAKE_CURRENT_SOURCE_DIR}/meshing/CGAL/cgal_utils.py
    ${CMAKE_CURRENT_SOURCE_DIR}/meshing/CGAL/mesh_from_point_cloud_using_convex_hull.py
    ${CMAKE_CURRENT_SOURCE_DIR}/meshing/CGAL/mesh_from_polyhedron.py
    ${CMAKE_CURRENT_SOURCE_DIR}/meshing/CGAL/data/point_cloud.pcd
    ${CMAKE_CURRENT_SOURCE_DIR}/meshing/CGAL/data/torus.obj
    ${CMAKE_CURRENT_SOURCE_DIR}/meshing/Gmsh/generateMesh.py
)

add_custom_target(${PROJECT_NAME} SOURCES ${EXAMPLES_FILES})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMPONENT resources
        DESTINATION share/SofaPython3/
        FILES_MATCHING PATTERN "*.py" PATTERN "*.pcd" PATTERN "*.obj"
        
)
project(Bindings.Component.Topology.Container.Grid)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/Topology/Container/Grid/Binding_RegularGridTopology.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/Topology/Container/Grid/Binding_RegularGridTopology_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/Topology/Container/Grid/Binding_SparseGridTopology.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/Topology/Container/Grid/Binding_SparseGridTopology_doc.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/Topology/Container/Grid/Binding_RegularGridTopology.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/Topology/Container/Grid/Binding_SparseGridTopology.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/Topology/Container/Grid/Module_Grid.cpp
)

SP3_add_component_binding(Sofa.Component.Topology.Container.Grid
    SOURCES ${SOURCE_FILES}
    HEADERS ${HEADER_FILES}
)

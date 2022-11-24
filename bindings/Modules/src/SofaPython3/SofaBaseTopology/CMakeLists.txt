project(Bindings.Modules.SofaBaseTopology)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_RegularGridTopology.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_SparseGridTopology.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Module_SofaBaseTopology.cpp
)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_RegularGridTopology.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_RegularGridTopology_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_SparseGridTopology.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_SparseGridTopology_doc.h
)

sofa_find_package(Sofa.Component.Topology.Container.Grid REQUIRED)

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    PACKAGE      Bindings.Modules
    MODULE       SofaBaseTopology
    DESTINATION  Sofa
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      Sofa.Component.Topology.Container.Grid SofaPython3::Plugin
)

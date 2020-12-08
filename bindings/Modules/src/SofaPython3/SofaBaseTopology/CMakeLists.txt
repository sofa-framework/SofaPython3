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

find_package(SofaBase REQUIRED)

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    PACKAGE_NAME Sofa
    MODULE_NAME  SofaBaseTopology
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      SofaBaseTopology SofaPython3::Plugin
)

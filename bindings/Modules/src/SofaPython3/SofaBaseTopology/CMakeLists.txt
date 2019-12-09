cmake_minimum_required(VERSION 3.1)
project(PythonModule_SofaBaseTopology)

set(SOURCE_FILES
    Binding_RegularGridTopology.cpp
    Binding_SparseGridTopology.cpp
    Module_SofaBaseTopology.cpp
)

set(HEADER_FILES
    Binding_RegularGridTopology.h
    Binding_RegularGridTopology_doc.h
    Binding_SparseGridTopology.h
    Binding_SparseGridTopology_doc.h
)

SP3_add_python_module(
    TARGET
        ${PROJECT_NAME}
    MODULE_NAME
        BaseTopology
    SOURCES
        ${SOURCE_FILES}
        ${HEADER_FILES}
    DEPENDS
        SofaBaseTopology SofaPython3
    DESTINATION
        ${SP3_PYTHON_PACKAGES_DIRECTORY}/Sofa
)
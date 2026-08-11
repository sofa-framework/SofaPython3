project(Bindings.Modules.SofaFEM)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Quadrature.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Module_SofaFEM.cpp
)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Quadrature.h
)

if (NOT TARGET SofaPython3::Plugin)
    find_package(SofaPython3 REQUIRED COMPONENTS Plugin Bindings.Sofa)
endif()

sofa_find_package(Sofa.FEM REQUIRED)

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    PACKAGE      Bindings.Modules
    MODULE       SofaFEM
    DESTINATION  Sofa
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      Sofa.FEM SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core
)

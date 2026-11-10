project(Bindings.Sofa.FEM)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Quadrature.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Quadrature.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_FEM.cpp
)

sofa_find_package(Sofa.FEM REQUIRED)

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    PACKAGE      Bindings.Sofa
    MODULE       FEM
    DESTINATION  Sofa
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      Sofa.FEM SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core
)

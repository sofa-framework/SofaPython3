project(Bindings.Modules.SofaLinearSolver)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_LinearSolver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_LinearSolver_doc.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_LinearSolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Module_SofaLinearSolver.cpp
)

if (NOT TARGET SofaPython3::Plugin)
    find_package(SofaPython3 REQUIRED COMPONENTS Plugin Bindings.Sofa)
endif()

sofa_find_package(Sofa.Component.LinearSolver.Iterative REQUIRED)

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    PACKAGE      Bindings.Modules
    MODULE       SofaLinearSolver
    DESTINATION  Sofa
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      Sofa.Component.LinearSolver.Iterative SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core
)

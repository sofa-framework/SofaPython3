project(Bindings.Modules.SofaConstraintSolver)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_ConstraintSolver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_ConstraintSolver_doc.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_ConstraintSolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Module_SofaConstraintSolver.cpp
)

if (NOT TARGET SofaPython3::Plugin)
    find_package(SofaPython3 REQUIRED COMPONENTS Plugin Bindings.Sofa)
endif()

find_package(Sofa.Component.Constraint.Lagrangian.Solver REQUIRED)

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    PACKAGE      Bindings.Modules
    MODULE       SofaConstraintSolver
    DESTINATION  Sofa
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      Sofa.Component.Constraint.Lagrangian.Solver SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core
)

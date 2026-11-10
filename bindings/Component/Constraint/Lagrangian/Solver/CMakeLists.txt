project(Bindings.Component.Constraint.Lagrangian.Solver)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/Constraint/Lagrangian/Solver/Binding_ConstraintSolver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/Constraint/Lagrangian/Solver/Binding_ConstraintSolver_doc.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/Constraint/Lagrangian/Solver/Binding_ConstraintSolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/Constraint/Lagrangian/Solver/Module_Solver.cpp
)

SP3_add_component_binding(Sofa.Component.Constraint.Lagrangian.Solver
    SOURCES ${SOURCE_FILES}
    HEADERS ${HEADER_FILES}
)

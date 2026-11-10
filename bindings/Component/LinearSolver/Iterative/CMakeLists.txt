project(Bindings.Component.LinearSolver.Iterative)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/LinearSolver/Iterative/Binding_LinearSolver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/LinearSolver/Iterative/Binding_LinearSolver_doc.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/LinearSolver/Iterative/Binding_LinearSolver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/LinearSolver/Iterative/Module_Iterative.cpp
)

SP3_add_component_binding(Sofa.Component.LinearSolver.Iterative
    SOURCES ${SOURCE_FILES}
    HEADERS ${HEADER_FILES}
)

project(Bindings.Modules.SofaSimulationCore)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_MechanicalOperations.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_VectorOperations.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Module_SofaSimulationCore.cpp
)

set(HEADER_FILES
)

find_package(Sofa.SimulationCore REQUIRED)

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    PACKAGE      Bindings
    MODULE       SofaSimulationCore
    DESTINATION  Sofa
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      SofaSimulationCore SofaPython3::Plugin
)

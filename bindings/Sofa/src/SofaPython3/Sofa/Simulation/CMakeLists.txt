project(Bindings.Sofa.Simulation)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Simulation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Simulation_doc.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Simulation.cpp
)

find_package(SofaFramework REQUIRED)
find_package(SofaSimulation REQUIRED)
find_package(SofaBase REQUIRED)

SP3_add_python_module(
        TARGET       ${PROJECT_NAME}
        PACKAGE_NAME Sofa
        MODULE_NAME  Simulation
        SOURCES      ${SOURCE_FILES}
        HEADERS      ${HEADER_FILES}
        DEPENDS      SofaCore SofaSimulationCore SofaSimulationGraph SofaBaseVisual
)
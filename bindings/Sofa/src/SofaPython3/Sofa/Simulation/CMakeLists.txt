project(Bindings.Sofa.Simulation)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Simulation_doc.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Simulation.cpp
)

if (NOT TARGET SofaPython3::Plugin)
    find_package(SofaPython3 REQUIRED COMPONENTS Plugin)
endif()

find_package(SofaFramework REQUIRED)
find_package(SofaSimulationGraph REQUIRED)

SP3_add_python_module(
        TARGET       ${PROJECT_NAME}
        PACKAGE      Bindings.Sofa
        MODULE       Simulation
        DESTINATION  Sofa
        SOURCES      ${SOURCE_FILES}
        HEADERS      ${HEADER_FILES}
        DEPENDS      Sofa.Core Sofa.SimulationCore SofaSimulationGraph SofaPython3::Plugin
)

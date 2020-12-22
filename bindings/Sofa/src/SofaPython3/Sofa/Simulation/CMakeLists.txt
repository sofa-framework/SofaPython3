project(Bindings.Sofa.Simulation)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Simulation_doc.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Simulation.cpp
)

if (NOT TARGET SofaPython3::Plugin)
    find_package(SofaPython3 REQUIRED)
endif()

find_package(SofaFramework REQUIRED)
find_package(SofaSimulation REQUIRED)

SP3_add_python_module(
        TARGET       ${PROJECT_NAME}
        PACKAGE_NAME Sofa
        MODULE_NAME  Simulation
        SOURCES      ${SOURCE_FILES}
        HEADERS      ${HEADER_FILES}
        DEPENDS      SofaCore SofaSimulationCore SofaSimulationGraph SofaPython3::Plugin
)

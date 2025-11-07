project(Bindings.Sofa.Simulation)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Simulation_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_SceneCheck.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_SceneCheck_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_SceneCheckMainRegistry.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_SceneCheckMainRegistry_doc.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Simulation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_SceneCheck.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_SceneCheckMainRegistry.cpp
)

if (NOT TARGET SofaPython3::Plugin)
    sofa_find_package(SofaPython3 REQUIRED COMPONENTS Plugin)
endif()

sofa_find_package(Sofa.Simulation.Graph REQUIRED)

SP3_add_python_module(
        TARGET       ${PROJECT_NAME}
        PACKAGE      Bindings.Sofa
        MODULE       Simulation
        DESTINATION  Sofa
        SOURCES      ${SOURCE_FILES}
        HEADERS      ${HEADER_FILES}
        DEPENDS      Sofa.Simulation.Graph SofaPython3::Plugin
)

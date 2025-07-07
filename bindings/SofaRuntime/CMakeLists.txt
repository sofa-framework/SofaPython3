project(Bindings.SofaRuntime)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaRuntime/Module_SofaRuntime.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaRuntime/Timer/Submodule_Timer.cpp
)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaRuntime/Timer/Submodule_Timer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaRuntime/Timer/Submodule_Timer_doc.h
)

sofa_find_package(Sofa.Simulation.Graph REQUIRED)

SP3_add_python_package(
    SOURCE_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}/package
    TARGET_DIRECTORY
        SofaRuntime
)

SP3_add_python_module(
        TARGET       ${PROJECT_NAME}
        MODULE       SofaRuntime
        DESTINATION  SofaRuntime
        SOURCES      ${SOURCE_FILES}
        HEADERS      ${HEADER_FILES}
        DEPENDS      Sofa.Simulation.Graph SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core
)

sofa_create_component_in_package_with_targets(
    COMPONENT_NAME ${PROJECT_NAME}
    COMPONENT_VERSION ${SofaPython3_VERSION}
    PACKAGE_NAME SofaPython3
    TARGETS ${PROJECT_NAME}
    )

if(SP3_BUILD_TEST)
    add_subdirectory(tests)
endif()

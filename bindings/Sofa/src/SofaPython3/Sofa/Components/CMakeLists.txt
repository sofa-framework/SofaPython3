project(Bindings.Sofa.Components)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Components.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Components.cpp
)

find_package(SofaFramework REQUIRED)

SP3_add_python_module(
        TARGET       ${PROJECT_NAME}
        PACKAGE_NAME Sofa
        MODULE_NAME  Components
        SOURCES      ${SOURCE_FILES}
        HEADERS      ${HEADER_FILES}
        DEPENDS      SofaCore SofaHelper SofaSimulationCore SofaPython3::Plugin
)
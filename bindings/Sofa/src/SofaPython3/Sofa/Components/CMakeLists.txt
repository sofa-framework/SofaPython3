project(Bindings.Sofa.Components VERSION 1.0)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Components.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Components.cpp
)

find_package(SofaFramework REQUIRED)
find_package(SofaBase REQUIRED)

SP3_add_python_module_2(
        TARGET       ${PROJECT_NAME}
        PACKAGE_NAME Sofa
        MODULE_NAME  Components
        SOURCES      ${SOURCE_FILES}
        HEADERS      ${HEADER_FILES}
        DEPENDS      SofaCore SofaPython3::Plugin
)
project(Bindings.Sofa.Types)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BoundingBox.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Types.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BoundingBox.cpp
)

find_package(SofaFramework REQUIRED)

SP3_add_python_module(
        TARGET       ${PROJECT_NAME}
        PACKAGE_NAME Sofa
        MODULE_NAME  Types
        SOURCES      ${SOURCE_FILES}
        HEADERS      ${HEADER_FILES}
        DEPENDS      SofaCore SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core
)
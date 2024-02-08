project(Bindings.Modules.SofaLinearSystem)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_LinearSystem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_LinearSystem_doc.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_LinearSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Module_SofaLinearSystem.cpp
)

if (NOT TARGET SofaPython3::Plugin)
    find_package(SofaPython3 REQUIRED COMPONENTS Plugin Bindings.Sofa)
endif()

sofa_find_package(Sofa.Component.LinearSystem REQUIRED)

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    PACKAGE      Bindings.Modules
    MODULE       SofaLinearSystem
    DESTINATION  Sofa
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      Sofa.Component.LinearSystem SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core
)

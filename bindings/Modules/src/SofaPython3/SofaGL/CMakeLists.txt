project(Bindings.Modules.SofaGL)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Module_SofaGL.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_DrawToolGL.cpp
)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_DrawToolGL.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_DrawToolGL_doc.h
)

if (NOT TARGET SofaPython3::Plugin)
    find_package(SofaPython3 REQUIRED COMPONENTS Plugin Bindings.Sofa)
endif()

find_package(Sofa.GL REQUIRED)

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    PACKAGE      Bindings.Modules
    MODULE       SofaGL
    DESTINATION  Sofa
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      Sofa.GL SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core

)

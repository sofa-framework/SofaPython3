project(Bindings.Sofa.GL)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_DrawToolGL.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_DrawToolGL_doc.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_DrawToolGL.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_GL.cpp
)

sofa_find_package(Sofa.GL REQUIRED)

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    PACKAGE      Bindings.Sofa
    MODULE       GL
    DESTINATION  Sofa
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      Sofa.GL SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core
)

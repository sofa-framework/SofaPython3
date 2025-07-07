project(Bindings.SofaExporter)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaExporter/Binding_STLExporter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaExporter/Binding_STLExporter_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaExporter/Binding_VisualModelOBJExporter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaExporter/Binding_VisualModelOBJExporter_doc.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaExporter/Binding_STLExporter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaExporter/Binding_VisualModelOBJExporter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaExporter/Module_SofaExporter.cpp
)

sofa_find_package(Sofa.Component.IO.Mesh REQUIRED)

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    MODULE       SofaExporter
    DESTINATION  .
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      Sofa.Component.IO.Mesh SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core
)

sofa_create_component_in_package_with_targets(
    COMPONENT_NAME ${PROJECT_NAME}
    COMPONENT_VERSION ${SofaPython3_VERSION}
    PACKAGE_NAME SofaPython3
    TARGETS ${PROJECT_NAME}
    )

project(Bindings.SofaExporter)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaExporter/Binding_STLExporter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaExporter/Binding_STLExporter_doc.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaExporter/Binding_STLExporter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaExporter/Module_SofaExporter.cpp
)

find_package(SofaExporter REQUIRED)

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    MODULE_NAME  SofaExporter
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      SofaExporter SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core
)
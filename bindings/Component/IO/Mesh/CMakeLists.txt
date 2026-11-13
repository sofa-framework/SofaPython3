project(Bindings.Component.IO.Mesh)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/IO/Mesh/Binding_STLExporter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/IO/Mesh/Binding_STLExporter_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/IO/Mesh/Binding_VisualModelOBJExporter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/IO/Mesh/Binding_VisualModelOBJExporter_doc.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/IO/Mesh/Binding_STLExporter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/IO/Mesh/Binding_VisualModelOBJExporter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/IO/Mesh/Module_Mesh.cpp
)

SP3_add_component_binding(Sofa.Component.IO.Mesh
    SOURCES ${SOURCE_FILES}
    HEADERS ${HEADER_FILES}
)

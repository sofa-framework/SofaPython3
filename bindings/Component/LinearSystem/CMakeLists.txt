project(Bindings.Component.LinearSystem)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/LinearSystem/Binding_LinearSystem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/LinearSystem/Binding_LinearSystem.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/LinearSystem/Binding_LinearSystem_doc.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/LinearSystem/Binding_LinearSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/LinearSystem/Module_LinearSystem.cpp
)

SP3_add_component_binding(Sofa.Component.LinearSystem
    SOURCES ${SOURCE_FILES}
    HEADERS ${HEADER_FILES}
)

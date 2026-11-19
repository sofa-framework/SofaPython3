project(Bindings.Component.SolidMechanics.Spring)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/SolidMechanics/Spring/Binding_LinearSpring.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/SolidMechanics/Spring/Binding_LinearSpring_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/SolidMechanics/Spring/Binding_SpringForceField.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/SolidMechanics/Spring/Binding_SpringForceField_doc.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/SolidMechanics/Spring/Binding_LinearSpring.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/SolidMechanics/Spring/Binding_SpringForceField.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/SolidMechanics/Spring/Module_Spring.cpp
)

SP3_add_component_binding(Sofa.Component.SolidMechanics.Spring
    SOURCES ${SOURCE_FILES}
    HEADERS ${HEADER_FILES}
)

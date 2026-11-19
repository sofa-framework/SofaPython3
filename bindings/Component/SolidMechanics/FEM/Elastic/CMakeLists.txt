project(Bindings.Component.SolidMechanics.FEM.Elastic)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/SolidMechanics/FEM/Elastic/Binding_CauchyStress.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/SolidMechanics/FEM/Elastic/Binding_LameParameters.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/SolidMechanics/FEM/Elastic/Binding_CauchyStress.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/SolidMechanics/FEM/Elastic/Binding_LameParameters.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Component/SolidMechanics/FEM/Elastic/Module_Elastic.cpp
)

SP3_add_component_binding(Sofa.Component.SolidMechanics.FEM.Elastic
    SOURCES ${SOURCE_FILES}
    HEADERS ${HEADER_FILES}
)

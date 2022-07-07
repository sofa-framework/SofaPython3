project(Bindings.Sofa.Components)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Components.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Components.cpp
)

find_package(Sofa.Simulation.Core REQUIRED)

SP3_add_python_module(
        TARGET       ${PROJECT_NAME}
        PACKAGE      Bindings.Sofa
        MODULE       Components
        DESTINATION  Sofa
        SOURCES      ${SOURCE_FILES}
        HEADERS      ${HEADER_FILES}
        DEPENDS      Sofa.Simulation.Core SofaPython3::Plugin
)

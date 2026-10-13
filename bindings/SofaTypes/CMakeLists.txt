project(Bindings.SofaTypes)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaTypes/Binding_Mat.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaTypes/Binding_Quat.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaTypes/Binding_Vec.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaTypes/Binding_Transform.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaTypes/Module_SofaTypes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaTypes/Binding_Mat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaTypes/Binding_Quat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaTypes/Binding_Vec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaTypes/Binding_Transform.cpp
)

SP3_add_python_package(
    SOURCE_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}/package
    TARGET_DIRECTORY
        SofaTypes
)

sofa_find_package(Sofa.DefaultType REQUIRED)

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    MODULE       SofaTypes
    DESTINATION  SofaTypes
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      Sofa.DefaultType SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core
)

sofa_create_component_in_package_with_targets(
    COMPONENT_NAME ${PROJECT_NAME}
    COMPONENT_VERSION ${SofaPython3_VERSION}
    PACKAGE_NAME SofaPython3
    TARGETS ${PROJECT_NAME}
    )

if(SP3_BUILD_TEST)
    add_subdirectory(tests)
endif()

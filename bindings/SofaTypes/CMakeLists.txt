project(Bindings.SofaTypes)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaTypes/Binding_Mat.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaTypes/Binding_Quat.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaTypes/Binding_Vec.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaTypes/Module_SofaTypes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaTypes/Binding_Mat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaTypes/Binding_Quat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaTypes/Binding_Vec.cpp
)

SP3_add_python_package(
    SOURCE_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}/package
    TARGET_DIRECTORY
        ${SP3_PYTHON_PACKAGES_DIRECTORY}/SofaTypes
)

find_package(SofaFramework REQUIRED)

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    PACKAGE_NAME SofaTypes
    MODULE_NAME  SofaTypes
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      SofaCore SofaDefaultType SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core
)

if(SP3_BUILD_TEST)
    add_subdirectory(tests)
endif()

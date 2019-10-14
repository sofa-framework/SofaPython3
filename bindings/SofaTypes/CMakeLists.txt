cmake_minimum_required(VERSION 3.1)
project(PythonModule_SofaTypes)

####################################################################################################
### Module files
####################################################################################################
set(HEADER_FILES
    #    src/SofaPython3/SofaTypes/Binding_BoundingBox.h
    #    src/SofaPython3/SofaTypes/Binding_Color.h
    #    src/SofaPython3/SofaTypes/Binding_Frame.h
    src/SofaPython3/SofaTypes/Binding_Mat.h
    src/SofaPython3/SofaTypes/Binding_Quat.h
    src/SofaPython3/SofaTypes/Binding_Vec.h
    )

set(SOURCE_FILES
    src/SofaPython3/SofaTypes/Module_SofaTypes.cpp
    #    src/SofaPython3/SofaTypes/Binding_BoundingBox.cpp
    #    src/SofaPython3/SofaTypes/Binding_Color.cpp
    #    src/SofaPython3/SofaTypes/Binding_Frame.cpp
    src/SofaPython3/SofaTypes/Binding_Mat.cpp
    src/SofaPython3/SofaTypes/Binding_Quat.cpp
    src/SofaPython3/SofaTypes/Binding_Vec.cpp
    )

set(MODULE_NAME SofaTypes)
set(PACKAGE_DIRECTORY  ${SP3_PYTHON_PACKAGES_DIRECTORY}/SofaTypes)

SP3_add_python_package(
    SOURCE_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}/package
    TARGET_DIRECTORY
        ${PACKAGE_DIRECTORY}
)

SP3_add_python_module(
    TARGET
        ${PROJECT_NAME}
    MODULE_NAME
        ${MODULE_NAME}
    SOURCES
        ${SOURCE_FILES}
        ${HEADER_FILES}
    DEPENDS
        SofaDefaultType
    DESTINATION
        ${PACKAGE_DIRECTORY}
)

if(SP3_BUILD_TEST)
    add_subdirectory(tests)
endif()

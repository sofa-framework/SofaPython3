cmake_minimum_required(VERSION 3.1)
project(PythonModule_SofaRuntime)

####################################################################################################
### Module dependencies
####################################################################################################
if (NOT TARGET SofaPython3)
    find_package(SofaPython3 REQUIRED)
endif()

####################################################################################################
### Module files
####################################################################################################
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaRuntime/Module_SofaRuntime.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaRuntime/Timer/Submodule_Timer.cpp
    )
set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaRuntime/Timer/Submodule_Timer.h
    )


set(MODULE_NAME SofaRuntime)
set(PACKAGE_DIRECTORY  ${SP3_PYTHON_PACKAGES_DIRECTORY})

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
        SofaCore SofaSimulationGraph SofaPython3 SofaPython3_Sofa
    DESTINATION
        ${PACKAGE_DIRECTORY}
)

#if(SP3_BUILD_TEST)
#    add_subdirectory(tests)
#endif()

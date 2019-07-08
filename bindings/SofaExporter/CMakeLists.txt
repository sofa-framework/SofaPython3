cmake_minimum_required(VERSION 3.1)
project(SofaPython3_SofaExporter)

####################################################################################################
### Module dependencies
####################################################################################################
if (NOT TARGET SofaPython3)
    find_package(SofaPython3 REQUIRED)
endif()
if (NOT TARGET SofaPython3_Sofa_Core)
    find_package(SofaPython3_Sofa_Core REQUIRED)
endif()

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaExporter/Binding_STLExporter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaExporter/Binding_STLExporter_doc.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaExporter/Binding_STLExporter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaExporter/Module_SofaExporter.cpp
)


set(MODULE_NAME SofaExporter)
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
        SofaCore SofaPython3 SofaPython3_Sofa_Core SofaExporter
    DESTINATION
        ${PACKAGE_DIRECTORY}
)

if(SP3_BUILD_TEST)
    add_subdirectory(tests)
endif()

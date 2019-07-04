cmake_minimum_required(VERSION 3.11)
project(SofaPython3 VERSION 1.0)

# Detect if SofaPython3 is a subproject of another project (eg. when compiled within Sofa)
if (NOT "${CMAKE_PROJECT_NAME}" STREQUAL "${PROJECT_NAME}")
    set(SP3_COMPILED_AS_SUBPROJECT 1)
    message(STATUS "${PROJECT_NAME} is compiled as a subproject to the external project '${CMAKE_PROJECT_NAME}'.")
    if("${CMAKE_PROJECT_NAME}" STREQUAL "Sofa")
        set(SP3_COMPILED_AS_SOFA_SUBPROJECT 1)
    endif()
else()
    unset(SP3_COMPILED_AS_SUBPROJECT)
endif ()

# CMAKE TOOLS
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")
include(SofaPython3Tools)

if (SP3_COMPILED_AS_SOFA_SUBPROJECT)
    if(PLUGIN_SOFAPYTHON)
        message(FATAL_ERROR "SofaPython3 cannot be built alongside SofaPython. Please set PLUGIN_SOFAPYTHON to OFF")
    endif()
endif()

# OPTIONS
if (SP3_COMPILED_AS_SOFA_SUBPROJECT)
    option(SP3_BUILD_TEST "Compile the automatic tests for SofaPython3, along with the gtest library." ${SOFA_BUILD_TESTS})
else()
    option(SP3_BUILD_TEST "Compile the automatic tests for SofaPython3, along with the gtest library." ON)
endif()

set(SP3_PYTHON_PACKAGES_DIRECTORY
    "lib/site-packages"
    CACHE PATH
    "Path to the directory containing the python packages defined in the binding projects. (default to lib/site-packages)"
    )

# BUILD OPTIONS
if (NOT SP3_COMPILED_AS_SUBPROJECT)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
    endif()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SofaFramework REQUIRED)
find_package(SofaSimulation REQUIRED)
find_package(SofaGeneral REQUIRED)


unset(PYTHON_EXECUTABLE)
find_package(Python 3.7 COMPONENTS Interpreter Development REQUIRED)
set(PYTHON_INCLUDE_DIRS ${Python_INCLUDE_DIR})
set(PYTHON_EXECUTABLE ${Python_EXECUTABLE})
set(PYBIND11_PYTHON_VERSION ${Python_VERSION})
find_package(pybind11 QUIET)
if(NOT pybind11_FOUND)#
  message("-- Cannot build the python module 'Sofa', missing the pybind software to generate the bindings.")
  return()
endif()

message("-- Python include dirs: ${PYTHON_INCLUDE_DIRS}")
message("-- Python libraries: ${PYTHON_LIBRARIES}")
message("-- Python library: ${PYTHON_LIBRARY}")

set(HEADER_FILES
    src/SofaPython3/config.h
    src/SofaPython3/initModule.h
    src/SofaPython3/PythonEnvironment.h
    src/SofaPython3/SceneLoaderPY3.h
)

set(SOURCE_FILES
    src/SofaPython3/initModule.cpp
    src/SofaPython3/PythonEnvironment.cpp
    src/SofaPython3/SceneLoaderPY3.cpp
)

set(EXTRA_FILES
    SofaPython3Config.cmake.in
    README.md
    )

if(SP3_BUILD_TEST)
    list(APPEND HEADER_FILES
        src/SofaPython3/PythonTest.h
    )
    list(APPEND SOURCE_FILES
        src/SofaPython3/PythonTest.cpp
    )
endif(SP3_BUILD_TEST)
    
add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} ${EXTRA_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_SOFAPYTHON3")

target_link_libraries(${PROJECT_NAME} PUBLIC SofaCore SofaSimulationCore SofaSimulationGraph SofaComponentGeneral)

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    # dlopen() is used on Linux for a workaround (see PythonEnvironnement.cpp)
    target_link_libraries(${PROJECT_NAME} PRIVATE dl)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<INSTALL_INTERFACE:include>")

target_link_libraries(${PROJECT_NAME} PRIVATE "${PYTHON_LIBRARY}" )
target_include_directories(${PROJECT_NAME} PRIVATE "${PYTHON_INCLUDE_DIR}")

## When installing, copy the content of the example directory into module/SofaPython3
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples DESTINATION modules/SofaPython3 COMPONENT resources)

## When installing, copy the target into module/SofaPython3
install(TARGETS ${PROJECT_NAME} DESTINATION modules/SofaPython3)

# The signature of sofa_create_package is
#     sofa_create_package(package_name, version, the_targets, <include_subdir>)
# This function assumes that there is a FooConfig.cmake.in file template in the source directory.
# This function is an all in one replacement for install(TARGET), install(EXPORT), and also add
# the example directory in the INSTALL set.
sofa_create_package(${PROJECT_NAME} ${PROJECT_VERSION} SofaPython3 "SofaPython3")

### Python binding
add_subdirectory(bindings)

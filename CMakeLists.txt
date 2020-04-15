cmake_minimum_required(VERSION 3.11)

project(SofaPython3)

# Manually define VERSION
set(SOFAPYTHON3_VERSION_MAJOR 1)
set(SOFAPYTHON3_VERSION_MINOR 0)
set(SOFAPYTHON3_VERSION_PATCH 0)
set(SOFAPYTHON3_VERSION ${SOFAPYTHON3_VERSION_MAJOR}.${SOFAPYTHON3_VERSION_MINOR}.${SOFAPYTHON3_VERSION_PATCH})

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
    set(ARCHIVE_OUTPUT_DIRECTORY lib)
    set(RUNTIME_OUTPUT_DIRECTORY bin)

    if(WIN32)
        set(LIBRARY_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
    else()
        set(LIBRARY_OUTPUT_DIRECTORY ${ARCHIVE_OUTPUT_DIRECTORY})
    endif()

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${ARCHIVE_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${LIBRARY_OUTPUT_DIRECTORY})
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


unset(PYTHON_EXECUTABLE)
find_package(Python 3.7 COMPONENTS Interpreter Development REQUIRED)
set(PYTHON_INCLUDE_DIRS ${Python_INCLUDE_DIR})
set(PYTHON_EXECUTABLE ${Python_EXECUTABLE})
set(PYBIND11_PYTHON_VERSION ${Python_VERSION})
find_package(pybind11 QUIET)
if(NOT pybind11_FOUND)
  message("-- Cannot build the python module 'Sofa', missing the pybind software to generate the bindings.")
  return()
endif()

message("-- Python executable: ${PYTHON_EXECUTABLE}")
message("-- Python version: ${PYBIND11_PYTHON_VERSION}")
message("-- Python include dirs: ${PYTHON_INCLUDE_DIRS}")
message("-- Python libraries: ${PYTHON_LIBRARIES}")
message("-- Python library: ${PYTHON_LIBRARY}")

# Unset the following variables to not confuse the user between the variables used by find_package(Python) and
# those used by find_package(pybind11). The later only uses the output of find_package(Python).
unset(PYTHON_EXECUTABLE)
unset(PYTHON_INCLUDE_DIRS)
unset(PYTHON_LIBRARIES)
unset(PYTHON_LIBRARY)

## When installing, copy the content of the example directory into module/SofaPython3
#install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples DESTINATION modules/SofaPython3 COMPONENT resources)

#set(EXAMPLES_FILES
#        examples/example-forcefield.py
#        examples/example-forcefield2.py
#        examples/example-scriptcontroller.py
#    )
#
#add_custom_target(Examples SOURCES ${EXAMPLES_FILES})

#set(DOCUMENTATION_FILES
#    README.md
#    License.md
#    docs/sphinx/source/index.rst
#    docs/sphinx/source/menu/install.rst
#    docs/sphinx/source/menu/Contributing.rst
#    docs/sphinx/source/menu/SofaPlugin.rst
#    docs/sphinx/source/menu/SofaModule.rst
#    docs/sphinx/source/menu/CustomModule.rst
#    )
#
#add_custom_target(Documentation SOURCES ${DOCUMENTATION_FILES})

add_subdirectory(Plugin)
add_subdirectory(bindings)


#SP3_add_python_package(
#        SOURCE_DIRECTORY
#            ${CMAKE_CURRENT_SOURCE_DIR}/splib
#        TARGET_DIRECTORY
#            ${SP3_PYTHON_PACKAGES_DIRECTORY}/splib
#)
#
#SP3_add_python_package(
#        SOURCE_DIRECTORY
#            ${CMAKE_CURRENT_SOURCE_DIR}/constants
#        TARGET_DIRECTORY
#            ${SP3_PYTHON_PACKAGES_DIRECTORY}/Sofa/constants
#)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/SofaPython3Config.cmake.in"
    "${CMAKE_BINARY_DIR}/cmake/SofaPython3/SofaPython3Config.cmake"
    INSTALL_DESTINATION
    "${LIBRARY_OUTPUT_DIRECTORY}/cmake/SofaPython3"
)
write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/cmake/SofaPython3/SofaPython3ConfigVersion.cmake
    VERSION ${SOFAPYTHON3_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(
    EXPORT SofaPython3Targets
    NAMESPACE SofaPython3::
    DESTINATION "${LIBRARY_OUTPUT_DIRECTORY}/cmake/SofaPython3"
    COMPONENT headers
)
install(FILES
    "${CMAKE_BINARY_DIR}/cmake/SofaPython3/SofaPython3Config.cmake"
    "${CMAKE_BINARY_DIR}/cmake/SofaPython3/SofaPython3ConfigVersion.cmake"
    DESTINATION
    "${LIBRARY_OUTPUT_DIRECTORY}/cmake/SofaPython3"
)
cmake_minimum_required(VERSION 3.12)

project(SofaPython3 VERSION 1.0.0)

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

# OPTIONS
include(CMakeDependentOption)

if (SP3_COMPILED_AS_SOFA_SUBPROJECT)
    option(SP3_BUILD_TEST "Compile the automatic tests for SofaPython3, along with the gtest library." ${SOFA_BUILD_TESTS})
else()
    option(SP3_BUILD_TEST "Compile the automatic tests for SofaPython3, along with the gtest library." ON)
endif()

find_package(SofaExporter QUIET)
CMAKE_DEPENDENT_OPTION(SP3_WITH_SOFAEXPORTER "Bind the SOFA exporter component." ON "SofaExporter_FOUND" OFF)

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

if(MSVC)
    # With C++17 (/std:c++17), to get MSVC to behave, you need /permissive-
    # see https://github.com/pybind/pybind11/issues/1616
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")

    # use M_PI in cmath.h
    add_definitions("-D_USE_MATH_DEFINES")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable use of folder for IDE like VS
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set the minimum python version to 3.7
set(PYBIND11_PYTHON_VERSION 3.7)

# Find Python3 executable and set PYTHON_EXECUTABLE before finding pybind11
# to be sure that pybind11 relies on the right Python version
find_package(Python3 ${PYBIND11_PYTHON_VERSION} COMPONENTS Interpreter REQUIRED)
set(PYTHON_EXECUTABLE ${Python3_EXECUTABLE})

# Set the minimum pybind11 version to 2.3 (before that the pybind11::embed target did not exist)
find_package(pybind11 2.3 CONFIG QUIET REQUIRED)

set(SP3_PYTHON_PACKAGES_DIRECTORY
    "python3/site-packages"
    CACHE STRING
    "Directory name where the python packages will be built and installed.
    This will be prepend to LIBRARY_OUTPUT_DIRECTORY (default to site-packages)"
)

# Get the Python's user site packages directory, or FASLE if not found
SP3_get_python_user_site()
CMAKE_DEPENDENT_OPTION(
    SP3_LINK_TO_USER_SITE
    "Install python packages to python's user site directory. The directory path can be changed by setting SP3_PYTHON_PACKAGES_LINK_DIRECTORY."
    ON "PYTHON_USER_SITE" OFF)

set(SP3_PYTHON_PACKAGES_LINK_DIRECTORY
    "${PYTHON_USER_SITE}"
    CACHE PATH
    "Path to the directory where the python packages will be installed (with symbolic links).
    (default to $(python3 -m site --user-site))"
)

message("-- Python version: ${PYTHON_VERSION_STRING}")
message("-- Python executable: ${PYTHON_EXECUTABLE}")
message("-- Python include dirs: ${PYTHON_INCLUDE_DIRS}")
message("-- Python libraries: ${PYTHON_LIBRARIES}")
message("-- Python user site: ${PYTHON_USER_SITE}")
message("-- pybind11 version: ${pybind11_VERSION}")
message("-- pybind11 config: ${pybind11_CONFIG}")

# When using python3 from XCode on MacOS, the RPath is wrongly set to XCode frameworks directory:
#   LC_LOAD_DYLIB   @rpath/Python3.framework/Versions/3.7/Python3
#   LC_RPATH        /Applications/Xcode.app/Contents/Developer/Library/Frameworks/Python3.framework/Versions/3.7/lib
# Hence LC_RPATH/LC_LOAD_DYLIB does not exists.
# Until this is fixed (not sure if it comes from pybind11, cmake or XCode), we can add another path to RPATH:
#   LC_LOAD_DYLIB   @rpath/Python3.framework/Versions/3.7/Python3
#   LC_RPATH        /Applications/Xcode.app/Contents/Developer/Library/Frameworks/Python3.framework/Versions/3.7/lib
#                   /Applications/Xcode.app/Contents/Developer/Library/Frameworks  <----- ADDED
# And now one combination of  LC_RPATH/LC_LOAD_DYLIB will be valid.
# This should't change anything for those that use other python libs than XCode (homebrew for example) since the
# LC_LOAD_DYLIB from XCode is quite unique.
if (APPLE)
    set(CMAKE_INSTALL_RPATH "/Applications/Xcode.app/Contents/Developer/Library/Frameworks")
    set(CMAKE_BUILD_RPATH "/Applications/Xcode.app/Contents/Developer/Library/Frameworks")
endif()

find_package(SofaFramework REQUIRED)

if (NOT SP3_COMPILED_AS_SUBPROJECT)
    get_filename_component(SOFA_ROOT_DIR "${SofaFramework_DIR}/../../.." ABSOLUTE)
    message(STATUS "SOFA Framework:\n\tVersion: ${SofaFramework_VERSION}\n\tLocation: ${SOFA_ROOT_DIR}")
endif()

sofa_add_plugin(Plugin SofaPython3 ON)
if(PLUGIN_SOFAPYTHON3)
    add_subdirectory(bindings)
    add_subdirectory(examples)
    add_subdirectory(docs)
endif()

SP3_add_python_package(
    SOURCE_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}/splib
    TARGET_DIRECTORY
        splib
)

SP3_add_python_package(
    SOURCE_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}/constants
    TARGET_DIRECTORY
        Sofa/constants
)

sofa_create_package(
    PACKAGE_NAME ${PROJECT_NAME}
    PACKAGE_VERSION ${PROJECT_VERSION}
    RELOCATABLE "plugins"
    )

if (SP3_LINK_TO_USER_SITE AND SP3_PYTHON_PACKAGES_LINK_DIRECTORY)
    file(GLOB directories RELATIVE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${SP3_PYTHON_PACKAGES_DIRECTORY}" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${SP3_PYTHON_PACKAGES_DIRECTORY}/*")
    # Create the site-package link directory if it doesn't exists yet
    install(DIRECTORY DESTINATION ${SP3_PYTHON_PACKAGES_LINK_DIRECTORY})
    foreach(directory ${directories})
        if(IS_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${SP3_PYTHON_PACKAGES_DIRECTORY}/${directory})
            if(WIN32)
                install(CODE "\
                execute_process( \
                    COMMAND ${CMAKE_COMMAND} -E copy_directory \
                    ${CMAKE_INSTALL_PREFIX}/${LIBRARY_OUTPUT_DIRECTORY}/${SP3_PYTHON_PACKAGES_DIRECTORY}/${directory}/ \
                    ${SP3_PYTHON_PACKAGES_LINK_DIRECTORY}/${directory}   \
                )"
            )
            else()
                install(CODE "\
                    execute_process( \
                        COMMAND ${CMAKE_COMMAND} -E create_symlink \
                        ${CMAKE_INSTALL_PREFIX}/${LIBRARY_OUTPUT_DIRECTORY}/${SP3_PYTHON_PACKAGES_DIRECTORY}/${directory}/ \
                        ${SP3_PYTHON_PACKAGES_LINK_DIRECTORY}/${directory}   \
                    )"
                )
            endif()
        endif()
    endforeach()
endif()

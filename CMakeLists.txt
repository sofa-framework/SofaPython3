cmake_minimum_required(VERSION 3.12)

project(SofaPython3 VERSION 20.12.00)

find_package(SofaFramework REQUIRED)

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

# If the plugin is compiled within SOFA, add an option to deactivate it, in which case the remaining
# of this file won't run and therefore will not generate any error if the requirements (python, pybind11)
# are not found.
if (SP3_COMPILED_AS_SOFA_SUBPROJECT)
    option(PLUGIN_SOFAPYTHON3 "Build the SofaPython3 plugin." ON)
    if (NOT PLUGIN_SOFAPYTHON3)
        return()
    else()
        message("Adding plugin SofaPython3")
    endif()
endif()

# CMAKE TOOLS
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")
include(SofaPython3Tools)

# OPTIONS
include(CMakeDependentOption)

# If SOFA_BUILD_TESTS exists and is OFF, then this option will be auto-disabled
cmake_dependent_option(SP3_BUILD_TEST "Compile the automatic tests for SofaPython3, along with the gtest library." ON "SOFA_BUILD_TESTS OR NOT DEFINED SOFA_BUILD_TESTS" OFF)
if(SP3_BUILD_TEST)
    # TODO: remove this when SOFA's GTest handles threads correctly
    find_package(Threads REQUIRED)
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

# Find Python3
find_package(Python ${PYBIND11_PYTHON_VERSION} COMPONENTS Interpreter Development REQUIRED)

# Save PYTHON_* vars
set(PYTHON_VERSION_RESET "${PYTHON_VERSION}")
set(PYTHON_EXECUTABLE_RESET "${PYTHON_EXECUTABLE}")
set(PYTHON_LIBRARIES_RESET "${PYTHON_LIBRARIES}")
set(PYTHON_INCLUDE_DIRS_RESET "${PYTHON_INCLUDE_DIRS}")
set(PYTHON_LIBRARY_RESET "${PYTHON_LIBRARY}")
set(PYTHON_INCLUDE_DIR_RESET "${PYTHON_INCLUDE_DIR}")

# Change PYTHON_* vars before pybind11 find_package
# to be sure that pybind11 relies on the right Python version
set(PYTHON_VERSION "${Python_VERSION}" CACHE STRING "" FORCE)
set(PYTHON_EXECUTABLE "${Python_EXECUTABLE}" CACHE FILEPATH "" FORCE)
set(PYTHON_LIBRARIES "${Python_LIBRARIES}" CACHE STRING "" FORCE)
set(PYTHON_INCLUDE_DIRS "${Python_INCLUDE_DIRS}" CACHE STRING "" FORCE)
if(EXISTS "${Python_LIBRARY}")
    set(PYTHON_LIBRARY "${Python_LIBRARY}" CACHE INTERNAL "" FORCE)
elseif(EXISTS "${Python_LIBRARIES}")
    set(PYTHON_LIBRARY "${Python_LIBRARIES}" CACHE INTERNAL "" FORCE)
endif()
if(EXISTS "${Python_INCLUDE_DIR}")
    set(PYTHON_INCLUDE_DIR "${Python_INCLUDE_DIR}" CACHE INTERNAL "" FORCE)
elseif(EXISTS "${Python_INCLUDE_DIRS}")
    set(PYTHON_INCLUDE_DIR "${Python_INCLUDE_DIRS}" CACHE INTERNAL "" FORCE)
endif()

# Set the minimum pybind11 version to 2.3 (before that the pybind11::embed target did not exist)
find_package(pybind11 2.3 CONFIG QUIET REQUIRED)

set(SP3_PYTHON_PACKAGES_DIRECTORY
    "python3/site-packages"
    CACHE STRING
    "Directory name where the python packages will be built and installed.
    This will be prepend to LIBRARY_OUTPUT_DIRECTORY (default to site-packages)"
)

# Get the Python's user site packages directory, or FALSE if not found
SP3_get_python_user_site()

# Add an option to automatically create links towards the python packages of SP3
# into the python's user site packages directory.
option(
    SP3_LINK_TO_USER_SITE
    "Install python packages to python's user site directory. The directory path can be changed by setting SP3_PYTHON_PACKAGES_LINK_DIRECTORY."
    OFF
)

set(SP3_PYTHON_PACKAGES_LINK_DIRECTORY
    "${PYTHON_USER_SITE}"
    CACHE PATH
    "Path to the directory where the python packages will be installed (with symbolic links).
    (default to $(python3 -m site --user-site))"
)

message(STATUS "Python:
    Version: ${Python_VERSION}
    Executable: ${Python_EXECUTABLE}
    Headers: ${Python_INCLUDE_DIRS}
    Libraries: ${Python_LIBRARIES}
    User site: ${PYTHON_USER_SITE}"
)
message(STATUS "pybind11:
    Version: ${pybind11_VERSION}
    Config: ${pybind11_CONFIG}"
)

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

if (NOT SP3_COMPILED_AS_SUBPROJECT)
    get_filename_component(SOFA_ROOT_DIR "${SofaFramework_DIR}/../../.." ABSOLUTE)
    message(STATUS "SOFA Framework:\n\tVersion: ${SofaFramework_VERSION}\n\tLocation: ${SOFA_ROOT_DIR}")
endif()

add_subdirectory(Plugin)
add_subdirectory(bindings)
add_subdirectory(examples)
add_subdirectory(docs)

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

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/CMake/SofaPython3Tools.cmake" "${CMAKE_BINARY_DIR}/lib/cmake/SofaPython3Tools.cmake" COPYONLY)
install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/CMake/SofaPython3Tools.cmake"
    DESTINATION lib/cmake/SofaPython3
    COMPONENT headers
)

if (SP3_LINK_TO_USER_SITE AND SP3_PYTHON_PACKAGES_LINK_DIRECTORY)
    file(GLOB directories RELATIVE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${SP3_PYTHON_PACKAGES_DIRECTORY}" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${SP3_PYTHON_PACKAGES_DIRECTORY}/*")
    # Create the site-package link directory if it doesn't exists yet
    install(DIRECTORY DESTINATION ${SP3_PYTHON_PACKAGES_LINK_DIRECTORY})
    foreach(directory ${directories})
        if(SP3_COMPILED_AS_SOFA_SUBPROJECT)
            set(LINK_SOURCE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/plugins/SofaPython3/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${directory})
        else()
            set(LINK_SOURCE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${directory})
        endif()

        if(IS_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${SP3_PYTHON_PACKAGES_DIRECTORY}/${directory})
            if(WIN32)
                install(CODE "\
                execute_process( \
                    COMMAND ${CMAKE_COMMAND} -E copy_directory \
                    ${LINK_SOURCE_DIRECTORY}/ \
                    ${SP3_PYTHON_PACKAGES_LINK_DIRECTORY}/${directory}   \
                )"
            )
            else()
                install(CODE "\
                    execute_process( \
                        COMMAND ${CMAKE_COMMAND} -E create_symlink \
                        ${LINK_SOURCE_DIRECTORY}/ \
                        ${SP3_PYTHON_PACKAGES_LINK_DIRECTORY}/${directory}   \
                    )"
                )
            endif()
        endif()
    endforeach()
endif()

# Reset PYTHON_* vars
set(PYTHON_VERSION "${PYTHON_VERSION_RESET}" CACHE STRING "" FORCE)
set(PYTHON_EXECUTABLE "${PYTHON_EXECUTABLE_RESET}" CACHE FILEPATH "" FORCE)
set(PYTHON_LIBRARIES "${PYTHON_LIBRARIES_RESET}" CACHE STRING "" FORCE)
set(PYTHON_INCLUDE_DIRS "${PYTHON_INCLUDE_DIRS_RESET}" CACHE STRING "" FORCE)
set(PYTHON_LIBRARY "${PYTHON_LIBRARY_RESET}" CACHE INTERNAL "" FORCE)
set(PYTHON_INCLUDE_DIR "${PYTHON_INCLUDE_DIR_RESET}" CACHE INTERNAL "" FORCE)

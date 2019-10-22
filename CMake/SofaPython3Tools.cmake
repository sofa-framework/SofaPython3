# Copy files from source directory to destination directory, substituting any
# variables.  Create destination directory if it does not exist.

macro(configure_files srcDir destDir)
    make_directory(${destDir})

    file(GLOB_RECURSE templateFiles RELATIVE ${srcDir} ${srcDir}/*)
    foreach(templateFile ${templateFiles})
        set(srcTemplatePath ${srcDir}/${templateFile})
        if(NOT IS_DIRECTORY ${srcTemplatePath})
            configure_file(
                    ${srcTemplatePath}
                    ${destDir}/${templateFile}
                    @ONLY)
        endif(NOT IS_DIRECTORY ${srcTemplatePath})
    endforeach(templateFile)
endmacro(configure_files)


# - Create a python package by copying the source directory to the destination directory. Every files within the
#   source directory will be configured with the current cmake variables available (see CMake configure_file documentation)
#
# SP3_add_python_package(PACKAGE_NAME SOURCE_DIRECTORY TARGET_DIRECTORY)
#  SOURCE_DIRECTORY   - (input) the source path of the directory to be configured and copied to the target directory.
#  TARGET_DIRECTORY   - (input) the target path of the directory that will contain the configured files.
#                               For the build tree, it is ${CMAKE_BINARY_DIR}/${TARGET_DIRECTORY}.
#                               For the install tree, it is ${CMAKE_INSTALL_PREFIX}/${TARGET_DIRECTORY}.
function(SP3_add_python_package)
    set(options)
    set(oneValueArgs PACKAGE_NAME SOURCE_DIRECTORY TARGET_DIRECTORY )
    set(multiValueArgs)

    cmake_parse_arguments(A "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    configure_files("${A_SOURCE_DIRECTORY}" "${CMAKE_BINARY_DIR}/${A_TARGET_DIRECTORY}")
    install(DIRECTORY "${CMAKE_BINARY_DIR}/${A_TARGET_DIRECTORY}" DESTINATION "${A_TARGET_DIRECTORY}")
    message(STATUS "Python package ${A_SOURCE_DIRECTORY} added to directory ${CMAKE_BINARY_DIR}/${A_TARGET_DIRECTORY}")
endfunction()

# - Create a target for a python module binding code with pybind11 support.
#
# SP3_add_python_module(TARGET MODULE_NAME DESTINATION SOURCES PYTHON_VERSION DEPENDS QUIET)
#  TARGET             - (input) the name of the generated target. Only used when cpp files are included in the sources.
#  MODULE_NAME        - (input) the name of the module.
#  DESTINATION        - (input) the output directory that will contain the compiled module.
#                               For the build tree, it is ${CMAKE_BINARY_DIR}/${DESTINATION}.
#                               For the install tree, it is ${CMAKE_INSTALL_PREFIX}/${DESTINATION}.
#  SOURCES            - (input) list of input files that will be compiled with pybind11 support.
#  PYTHON_VERSION     - (input) python version, default to the highest version found
#  DEPENDS            - (input) set of target the generated target will depends on.
#  QUIET              - (input) if set, not information messages will be printed out.
function(SP3_add_python_module)
    set(options QUIET)
    set(oneValueArgs TARGET MODULE_NAME DESTINATION PYTHON_VERSION )
    set(multiValueArgs SOURCES DEPENDS)

    cmake_parse_arguments(A "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (A_DESTINATION)
        set(DESTINATION "${A_DESTINATION}")
    else()
        set(DESTINATION "")
    endif ()


    if (A_SOURCES)

        if (NOT DEFINED A_MODULE_NAME)
            message(FATAL_ERROR "A module name must be provided.")
        endif()

        set(MODULE_NAME ${A_MODULE_NAME})

        if (NOT DEFINED A_TARGET)
            set(A_TARGET ${MODULE_NAME})
        endif()

        if (A_PYTHON_VERSION)
            set(PYBIND11_PYTHON_VERSION ${A_PYTHON_VERSION})
        endif()

        set(PYBIND11_CPP_STANDARD -std=c++17)

        find_package(pybind11 CONFIG REQUIRED)

        pybind11_add_module(${A_TARGET} SHARED "${A_SOURCES}")

        target_include_directories(${A_TARGET}
                PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>"
                PUBLIC $<INSTALL_INTERFACE:include>
                )

        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
            target_compile_options(${A_TARGET} PRIVATE -fsized-deallocation)
        endif()

        if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
            target_compile_options(${A_TARGET} PRIVATE -Dregister=)
        endif()

        target_link_libraries(${A_TARGET} PRIVATE ${A_DEPENDS} ${PYTHON_LIBRARIES} pybind11::module)

        set_target_properties(
                ${A_TARGET}
                PROPERTIES
                OUTPUT_NAME ${MODULE_NAME}
                LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${DESTINATION}"


        )

        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
            set_target_properties(
                ${A_TARGET}
                PROPERTIES
                # https://cmake.org/cmake/help/latest/prop_tgt/LIBRARY_OUTPUT_DIRECTORY_CONFIG.html#prop_tgt:LIBRARY_OUTPUT_DIRECTORY_<CONFIG>
                RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${DESTINATION}"
                RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${DESTINATION}"
                RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${DESTINATION}"
                RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/${DESTINATION}"
                RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/${DESTINATION}"
            )
        endif()

        install(TARGETS ${A_TARGET} LIBRARY DESTINATION ${DESTINATION})

        if (TARGET ALL_BINDINGS)
            add_dependencies(ALL_BINDINGS ${A_TARGET})
        endif()

        if (NOT A_QUIET)
            message(STATUS "Python module '${MODULE_NAME}' added to ${DESTINATION} (python version ${PYTHON_VERSION_STRING}, pybind11 version ${pybind11_VERSION})")
        endif ()

    endif()

endfunction()

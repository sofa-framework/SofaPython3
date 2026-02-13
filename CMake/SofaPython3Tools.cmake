include_guard()

# - Recursively configure files inside a directory. Takes the same argument as configure_file, but here the input
#   parameter is a path to a directory.
function(SP3_configure_directory input_folder output_folder)
    file(GLOB_RECURSE files RELATIVE ${input_folder} ${input_folder}/*)
    foreach(file_relative_path ${files})
        set(file_absolute_path ${input_folder}/${file_relative_path})
        configure_file(
            ${file_absolute_path}
            ${output_folder}/${file_relative_path}
            ${ARGN}
        )
    endforeach()
endfunction()

# - Get the path to the Python's user site packages directory (for example, $HOME/.local/lib/pythonX.Y/site-packages
# PYTHON_USER_SITE - (output) The path to Python's site packages directory, or FALSE if not found
macro(SP3_get_python_user_site)
    execute_process(
            COMMAND "${PYTHON_EXECUTABLE}" "-m" "site" "--user-site"
            RESULT_VARIABLE _SITE_PACKAGE_SUCCESS
            OUTPUT_VARIABLE PYTHON_USER_SITE
    )

    if(_SITE_PACKAGE_SUCCESS MATCHES 0)
        string(REGEX REPLACE "\n" "" PYTHON_USER_SITE ${PYTHON_USER_SITE})
        set(PYTHON_USER_SITE "${PYTHON_USER_SITE}")

        # Create the user-site if it does not yet exists
        file(MAKE_DIRECTORY "${PYTHON_USER_SITE}")
    else()
        set(PYTHON_USER_SITE FALSE)
    endif()
endmacro()

# - Create a python package by copying the source directory to the destination directory.
#
# SP3_add_python_package(PACKAGE_NAME SOURCE_DIRECTORY TARGET_DIRECTORY)
#  SOURCE_DIRECTORY   - (input) the source path of the directory to be copied to the target directory.
#  TARGET_DIRECTORY   - (input) the target path of the directory that will contain the copied files.
#                               Files will be at LIBRARY_OUTPUT_DIRECTORY/SP3_PYTHON_PACKAGES_DIRECTORY/TARGET_DIRECTORY.
function(SP3_add_python_package)
    set(options)
    set(oneValueArgs PACKAGE_NAME SOURCE_DIRECTORY TARGET_DIRECTORY )
    set(multiValueArgs)

    cmake_parse_arguments(A "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${A_TARGET_DIRECTORY})

    file(GLOB_RECURSE files RELATIVE ${A_SOURCE_DIRECTORY} ${A_SOURCE_DIRECTORY}/*)
    foreach(file_relative_path ${files})
        set(file_absolute_path ${A_SOURCE_DIRECTORY}/${file_relative_path})
        get_filename_component(relative_directory ${file_relative_path} DIRECTORY)
        file(COPY ${file_absolute_path} DESTINATION ${OUTPUT_DIRECTORY}/${relative_directory})
        install(
            FILES "${OUTPUT_DIRECTORY}/${file_relative_path}"
            DESTINATION "lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${A_TARGET_DIRECTORY}/${relative_directory}"
        )
    endforeach()

endfunction()

# - Create a target for a python module binding code with pybind11 support.
#
# SP3_add_python_module(TARGET MODULE_NAME DESTINATION SOURCES PYTHON_VERSION DEPENDS QUIET)
#  TARGET             - (input) The name of the generated target. Only used when cpp files are included in the sources.
#  PACKAGE            - (input) The name of the cmake package that will contain this target. Note that the package must
#                               be created later on during the cmake configure stage using the macro sofa_create_package(),
#                               or be manually created using cmake's install command with "Targets" as suffix, e.g.:
#                               install(EXPORT ${PACKAGE}Targets).
#  MODULE             - (input) The name of the module. This is the name that will be used in python to do the actual
#                               import of the python module containing the bindings, i.e. "from MODULE import *".
#  DESTINATION        - (input) The output directory that will contain the compiled module.
#                               For the build tree, it will be ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${SP3_PYTHON_PACKAGES_BUILD_DIRECTORY}/${DESTINATION}.
#                               For the install tree, it will be ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${SP3_PYTHON_PACKAGES_INSTALL_DIRECTORY}/${DESTINATION}.
#  SOURCES            - (input) list of source files that will be compiled with pybind11 support.
#  HEADERS            - (input) list of header files that will be installed after the build.
#  DEPENDS            - (input) set of target the generated target will depends on.
#  QUIET              - (input) if set, not information messages will be printed out.
function(SP3_add_python_module)
    set(options QUIET)
    set(oneValueArgs TARGET PACKAGE MODULE DESTINATION PYTHON_VERSION )
    set(multiValueArgs SOURCES HEADERS DEPENDS)

    cmake_parse_arguments(A "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(DESTINATION "${A_DESTINATION}")

    # Fetch the current path relative to /bindings/*/src
    # Must test if the result is nested into the CMAKE_CURRENT_SOURCE_DIR (case where src exists outside the plugin directory)
    string(REGEX MATCH "(.*)/src" path_to_src "${CMAKE_CURRENT_SOURCE_DIR}")
    if ( ( NOT path_to_src OR "${path_to_src}" STRLESS "${CMAKE_CURRENT_SOURCE_DIR}" ) AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src" )
        set(path_to_src "${CMAKE_CURRENT_SOURCE_DIR}/src")
    endif()

    if (NOT A_SOURCES)
        message(FATAL_ERROR "Cannot generate a python module without source files.")
    endif()


    if (NOT DEFINED A_MODULE)
        message(FATAL_ERROR "A module name must be provided.")
    endif()

    set(MODULE_NAME ${A_MODULE})

    if (NOT DEFINED A_TARGET)
        set(A_TARGET ${MODULE_NAME})
    endif()

    set(PYBIND11_CPP_STANDARD -std=c++17)

    find_package(pybind11 CONFIG QUIET REQUIRED)

    # We are doing manually what's usually done with pybind11_add_module(${A_TARGET} SHARED "${A_SOURCES}")
    # since we got some problems on MacOS using recent versions of pybind11 where the SHARED argument wasn't taken
    # into account
    python_add_library(${A_TARGET} SHARED ${A_HEADERS} ${A_SOURCES})
    add_library(SofaPython3::${A_TARGET} ALIAS ${A_TARGET})

    if ("${pybind11_VERSION}" VERSION_GREATER_EQUAL "2.6.0")
        target_link_libraries(${A_TARGET} PRIVATE pybind11::headers)
        target_link_libraries(${A_TARGET} PRIVATE pybind11::embed)
        target_link_libraries(${A_TARGET} PRIVATE pybind11::lto)
        if(MSVC)
            target_link_libraries(${A_TARGET} PRIVATE pybind11::windows_extras)
        endif()

        pybind11_extension(${A_TARGET})
        pybind11_strip(${A_TARGET})
    else()
        target_link_libraries(${A_TARGET} PRIVATE pybind11::module)

        # Equivalent to pybind11_extension(${A_TARGET}) which doesn't exists on pybind11 versions < 5
        set_target_properties(${A_TARGET} PROPERTIES PREFIX "" SUFFIX "${PYTHON_MODULE_EXTENSION}")

        if(NOT MSVC AND NOT ${CMAKE_BUILD_TYPE} MATCHES Debug|RelWithDebInfo)
            # Equivalent to pybind11_strip(${A_TARGET}) which doesn't exists on pybind11 versions < 5
            # Strip unnecessary sections of the binary on Linux/macOS
            if(CMAKE_STRIP)
                if(APPLE)
                    set(x_opt -x)
                endif()

                add_custom_command(
                        TARGET ${A_TARGET}
                        POST_BUILD
                        COMMAND ${CMAKE_STRIP} ${x_opt} $<TARGET_FILE:${A_TARGET}>)
            endif()
        endif()
    endif()

    set_target_properties(${A_TARGET}
        PROPERTIES
            CXX_VISIBILITY_PRESET "hidden"
            CUDA_VISIBILITY_PRESET "hidden")

    target_include_directories(${A_TARGET}
        PUBLIC "$<BUILD_INTERFACE:${path_to_src}/>"
        PUBLIC $<INSTALL_INTERFACE:include>
    )



    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") # Clang or AppleCLang
        target_compile_options(${A_TARGET} PUBLIC -fsized-deallocation)
    endif()

    if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        target_compile_options(${A_TARGET} PRIVATE -Dregister=)
    endif()

    target_link_libraries(${A_TARGET} PUBLIC "${A_DEPENDS}")

    set_target_properties(
        ${A_TARGET}
        PROPERTIES
            OUTPUT_NAME ${MODULE_NAME}
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}"
    )
    get_target_property(TARGET_LIBRARY_OUTPUT_DIRECTORY ${A_TARGET} LIBRARY_OUTPUT_DIRECTORY)

    sofa_get_target_dependencies(DEPENDS_ALL ${A_TARGET})

    # Compute the installation RPATHs from the target's SP3 dependencies since they are not installed in a same directory
    # and are not automatically added from the cmake option INSTALL_RPATH_USE_LINK_PATH.
    # 1. Get all dependencies that are
    #    (a) a target in this project, and
    #    (b) built in  CMAKE_LIBRARY_OUTPUT_DIRECTORY
    # 2. Here we are sure that the dependency is a SP3 target and not an imported target from an external dependency.
    #    We compute its path relative to this target output file
    #    Ex: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/site-packages/Sofa  --> $ORIGIN/../Sofa
    # 3. Add the relative path computed in 2 to the list of RPATHS
    get_target_property(TARGET_DEPENDENCIES_RPATH ${A_TARGET} "INSTALL_RPATH")

    # Get the relative path from this binding module to the install lib directory
    # For example, for lib/python3/site-packages/Sofa/Core.***.so, the relative path will be
    # "../../.."
    file(RELATIVE_PATH from_target_to_lib "${TARGET_LIBRARY_OUTPUT_DIRECTORY}" "${CMAKE_BINARY_DIR}/lib")
    file(TO_CMAKE_PATH "${from_target_to_lib}" from_target_to_lib) # prettify this path

    # RPATH needed to find dependencies in <SofaPython3_install_dir>/lib
    list(APPEND TARGET_DEPENDENCIES_RPATH
        "$ORIGIN/${from_target_to_lib}/../lib"
        "$$ORIGIN/${from_target_to_lib}/../lib"
        "@loader_path/${from_target_to_lib}/../lib"
        "@executable_path/${from_target_to_lib}/../lib"
        )

    # RPATH needed to find dependencies in <SOFA_install_dir>/lib
    list(APPEND TARGET_DEPENDENCIES_RPATH
        "$ORIGIN/${from_target_to_lib}/../../../lib"
        "$$ORIGIN/${from_target_to_lib}/../../../lib"
        "@loader_path/${from_target_to_lib}/../../../lib"
        "@executable_path/${from_target_to_lib}/../../../lib"
        )

    if (APPLE)
        # In MacOS, the target dependency name is RPATH/site-packages/PackageName, so we need to add
        # an RPATH to the directory that contains "site-packages"
        list(APPEND TARGET_DEPENDENCIES_RPATH
            "$ORIGIN/../.."
            "$$ORIGIN/../.."
            "@loader_path/../.."
            "@executable_path/../.."
            )
    endif()

    # RPATH needed to find dependencies in <SofaPython3_install_dir>/lib/python3/site-packages
    foreach(DEPENDENCY ${DEPENDS_ALL})
        if(NOT TARGET ${DEPENDENCY})
            continue()
        endif()
        get_target_property(aliased_dep ${DEPENDENCY} ALIASED_TARGET)
        if(aliased_dep)
            set(DEPENDENCY ${aliased_dep})
        endif()
        get_target_property(DEPENDENCY_LIBRARY_OUTPUT_DIRECTORY "${DEPENDENCY}" LIBRARY_OUTPUT_DIRECTORY)
        # if dependency is also a python module
        if(DEPENDENCY_LIBRARY_OUTPUT_DIRECTORY MATCHES ".*${SP3_PYTHON_PACKAGES_DIRECTORY}.*")
            file(RELATIVE_PATH from_target_to_dependency "${TARGET_LIBRARY_OUTPUT_DIRECTORY}" "${DEPENDENCY_LIBRARY_OUTPUT_DIRECTORY}")
            file(TO_CMAKE_PATH "${from_target_to_dependency}" from_target_to_dependency) # prettify this path

            if(from_target_to_dependency)
                if(NOT "@loader_path/${from_target_to_dependency}" IN_LIST TARGET_DEPENDENCIES_RPATH)
                    list(APPEND TARGET_DEPENDENCIES_RPATH
                        "$ORIGIN/${from_target_to_dependency}"
                        "$$ORIGIN/${from_target_to_dependency}"
                        "@loader_path/${from_target_to_dependency}"
                        "@executable_path/${from_target_to_dependency}"
                        )
                endif()
            endif()
        endif()
    endforeach()

    # Compute the installation RPATHs from the target's SOFA relocatable dependencies
    # 1. First, compute the relative path from the current target towards the "plugins" relocatable directory of SOFA
    # 2. Append to the previous computed path the RELOCATABLE_INSTALL_DIR target property of the dependency since the
    #    latter is formulated with respect to the "plugins" relocatable (e.g. "plugins/SofaBoundaryConditions/lib")
    foreach(DEPENDENCY ${DEPENDS_ALL})
        if(NOT TARGET ${DEPENDENCY})
            continue()
        endif()
        get_target_property(aliased_dep ${DEPENDENCY} ALIASED_TARGET)
        if(aliased_dep)
            set(DEPENDENCY ${aliased_dep})
        endif()
        get_target_property(DEPENDENCY_RELOCATABLE_INSTALL_DIR "${DEPENDENCY}" RELOCATABLE_INSTALL_DIR)
        if (DEPENDENCY_RELOCATABLE_INSTALL_DIR)
            # Here, we assume that the SP3 plugin will be installed in the SOFA plugins directory (i.e. $SOFA_ROOT/plugins)
            # Hence, we need to compute the relative path from this plugins directory to the current binding
            # modules, for example, plugins/SofaPython3/lib/python3/site-packages/Sofa/Core.***.so
            # will become "../../../../../.." (three levels upper than the previous computed relative path)

            # Alright, now we have the path from the current target towards the "plugins" relocatable directory of SOFA
            # We can compute the relative path from the current target towards the dependency relocatable path.
            list(APPEND TARGET_DEPENDENCIES_RPATH
                "$ORIGIN/${from_target_to_lib}/../../../${DEPENDENCY_RELOCATABLE_INSTALL_DIR}/lib"
                "$$ORIGIN/${from_target_to_lib}/../../../${DEPENDENCY_RELOCATABLE_INSTALL_DIR}/lib"
                "@loader_path/${from_target_to_lib}/../../../${DEPENDENCY_RELOCATABLE_INSTALL_DIR}/lib"
                "@executable_path/${from_target_to_lib}/../../../${DEPENDENCY_RELOCATABLE_INSTALL_DIR}/lib"
                )
        endif()
    endforeach()

    set_target_properties(
        ${A_TARGET}
        PROPERTIES

            # This option only works for target that are not defined by the SP3 project
            # see https://stackoverflow.com/a/30400628 for details
            INSTALL_RPATH_USE_LINK_PATH TRUE

            # This will set the remaining RPATHs from our Bindings targets dependencies (install/lib/site-packages/*)
            INSTALL_RPATH "${TARGET_DEPENDENCIES_RPATH}"

            # Don't use the installation RPATH for built files
            BUILD_WITH_INSTALL_RPATH FALSE
    )

    if (APPLE)
        set_target_properties(
            ${PROJECT_NAME}
            PROPERTIES
            INSTALL_NAME_DIR "@rpath/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}"
        )
    endif()

    if(CMAKE_CONFIGURATION_TYPES) # for Multiconfiguration setups like MSVC or XCode
        set_target_properties(
            ${A_TARGET}
            PROPERTIES
                # https://cmake.org/cmake/help/latest/prop_tgt/LIBRARY_OUTPUT_DIRECTORY_CONFIG.html#prop_tgt:LIBRARY_OUTPUT_DIRECTORY_<CONFIG>
                RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}"
                RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}"
                RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}"
                RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}"
                RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}"

                LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}"
                LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}"
                LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}"
                LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}"
                LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}"

                ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}/libraries"
                ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}/libraries"
                ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}/libraries"
                ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}/libraries"
                ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}/libraries"
        )
    endif()


    if (A_PACKAGE)
        install(
            TARGETS ${A_TARGET}
            EXPORT ${A_PACKAGE}Targets
            RUNTIME DESTINATION "lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}" COMPONENT applications
            LIBRARY DESTINATION "lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}" COMPONENT libraries
            ARCHIVE DESTINATION "lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}" COMPONENT libraries
        )
    elseif (DESTINATION)
        install(
            TARGETS ${A_TARGET}
            RUNTIME DESTINATION "lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}" COMPONENT applications
            LIBRARY DESTINATION "lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}" COMPONENT libraries
            ARCHIVE DESTINATION "lib/${SP3_PYTHON_PACKAGES_DIRECTORY}/${DESTINATION}" COMPONENT libraries
        )
    endif()

    foreach(header ${A_HEADERS})
        file(RELATIVE_PATH path_from_package "${path_to_src}" "${header}")
        get_filename_component(dir_from_package ${path_from_package} DIRECTORY)

        install(
            FILES
                "${header}"
            DESTINATION
                "include/${dir_from_package}"
            COMPONENT headers
        )
    endforeach()

endfunction()

project(Bindings.Modules)

# Backward compatibility with the layout used before the bindings followed SOFA's Sofa.Component tree.
#
# - python: Sofa.SofaBaseTopology, Sofa.SofaDeformable, Sofa.SofaLinearSolver, Sofa.SofaLinearSystem,
#   Sofa.SofaConstraintSolver and SofaExporter are now thin modules re-exporting the new
#   Sofa.Component.* modules (with a deprecation message).
# - cmake: the Bindings.Modules component is an alias of Bindings.Component.

SP3_add_python_package(
    SOURCE_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}/python
    TARGET_DIRECTORY
        .
)

# Compiled modules left over from a previous build would shadow the python modules above (python
# loads extension modules before source files): remove them from the build tree.
set(SP3_SITE_PACKAGES_BUILD_DIR "${CMAKE_BINARY_DIR}/lib/${SP3_PYTHON_PACKAGES_DIRECTORY}")
set(SP3_LEGACY_COMPILED_MODULES
    Sofa/SofaBaseTopology
    Sofa/SofaConstraintSolver
    Sofa/SofaDeformable
    Sofa/SofaFEM
    Sofa/SofaGL
    Sofa/SofaLinearSolver
    Sofa/SofaLinearSystem
    SofaExporter
)
foreach(legacy_module ${SP3_LEGACY_COMPILED_MODULES})
    file(GLOB legacy_files "${SP3_SITE_PACKAGES_BUILD_DIR}/${legacy_module}.*")
    list(FILTER legacy_files EXCLUDE REGEX "\\.py$")
    if(legacy_files)
        message(STATUS "Removing outdated python module ${legacy_files}")
        file(REMOVE ${legacy_files})
    endif()
endforeach()

add_library(${PROJECT_NAME} INTERFACE)
add_library(SofaPython3::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} INTERFACE Bindings.Component)

sofa_create_component_in_package_with_targets(
    COMPONENT_NAME ${PROJECT_NAME}
    COMPONENT_VERSION ${SofaPython3_VERSION}
    PACKAGE_NAME SofaPython3
    TARGETS ${PROJECT_NAME}
    )

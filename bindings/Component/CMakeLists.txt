project(Bindings.Component)

# Python bindings for SOFA's Sofa.Component.* modules.
#
# The directory layout mirrors SOFA's Sofa/Component tree: the bindings of the SOFA module
# Sofa.Component.<Area>.<Sub> live in <Area>/<Sub>/ and are compiled into the python module
# Sofa.Component.<Area>.<Sub> (i.e. site-packages/Sofa/Component/<Area>/<Sub>.<ext>).
#
# A binding is only built when the SOFA module it binds is available.

# - Create the python module binding a SOFA Sofa.Component.* module.
#
# SP3_add_component_binding(<SOFA module name> SOURCES ... HEADERS ... [DEPENDS ...])
#  The target name is the current PROJECT_NAME (Bindings.Component.<Area>.<Sub>), the python module is
#  the last part of the SOFA module name, installed in the python package made of the other parts.
function(SP3_add_component_binding sofa_module)
    cmake_parse_arguments(A "" "" "SOURCES;HEADERS;DEPENDS" ${ARGN})

    string(REPLACE "." ";" module_path "${sofa_module}")
    list(POP_BACK module_path module_name)
    string(REPLACE ";" "/" module_destination "${module_path}")

    SP3_add_python_module(
        TARGET       ${PROJECT_NAME}
        PACKAGE      Bindings.Component
        MODULE       ${module_name}
        DESTINATION  ${module_destination}
        SOURCES      ${A_SOURCES}
        HEADERS      ${A_HEADERS}
        DEPENDS      ${sofa_module} ${A_DEPENDS} SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Bindings/Component")
    set_property(GLOBAL APPEND PROPERTY SP3_COMPONENT_BINDING_TARGETS ${PROJECT_NAME})
    set_property(GLOBAL APPEND PROPERTY SP3_COMPONENT_BINDING_SOFA_MODULES ${sofa_module})
endfunction()

if (NOT TARGET SofaPython3::Plugin)
    find_package(SofaPython3 REQUIRED COMPONENTS Plugin Bindings.Sofa)
endif()

set(COMPONENTBINDINGS_MODULE_LIST
    Constraint/Lagrangian/Solver
    LinearSolver/Iterative
    LinearSystem
    SolidMechanics/FEM/Elastic
    SolidMechanics/Spring
    Topology/Container/Grid
)
if(SP3_WITH_SOFAEXPORTER)
    list(APPEND COMPONENTBINDINGS_MODULE_LIST IO/Mesh)
endif()

set_property(GLOBAL PROPERTY SP3_COMPONENT_BINDING_TARGETS "")
set_property(GLOBAL PROPERTY SP3_COMPONENT_BINDING_SOFA_MODULES "")
foreach(binding_dir ${COMPONENTBINDINGS_MODULE_LIST})
    string(REPLACE "/" "." sofa_module "Sofa.Component.${binding_dir}")
    sofa_find_package(${sofa_module} QUIET)
    if(${sofa_module}_FOUND)
        message(STATUS "Adding binding module ${sofa_module}")
        add_subdirectory(${binding_dir})
    else()
        message(STATUS "${sofa_module} not found: its python bindings will not be compiled.")
    endif()
endforeach()
get_property(COMPONENTBINDINGS_TARGETS GLOBAL PROPERTY SP3_COMPONENT_BINDING_TARGETS)
get_property(COMPONENTBINDINGS_SOFA_MODULES GLOBAL PROPERTY SP3_COMPONENT_BINDING_SOFA_MODULES)

add_library(${PROJECT_NAME} INTERFACE)
add_library(SofaPython3::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} INTERFACE ${COMPONENTBINDINGS_TARGETS})

# Deprecated names (Sofa.SofaDeformable, SofaExporter, Bindings.Modules, ...)
add_subdirectory(legacy)

if(SP3_BUILD_TEST)
    add_subdirectory(tests)
endif()

sofa_create_component_in_package_with_targets(
    COMPONENT_NAME ${PROJECT_NAME}
    COMPONENT_VERSION ${SofaPython3_VERSION}
    PACKAGE_NAME SofaPython3
    TARGETS ${PROJECT_NAME}
    )

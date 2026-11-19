project(Bindings.Sofa)

set(SOFABINDINGS_MODULE_LIST
    Core
    Helper
    Simulation
    Types
    )

# Optional framework modules
sofa_find_package(Sofa.FEM QUIET)
if(Sofa.FEM_FOUND)
    list(APPEND SOFABINDINGS_MODULE_LIST FEM)
else()
    message(STATUS "Sofa.FEM not found: python bindings Sofa.FEM will not be compiled.")
endif()
sofa_find_package(Sofa.GL QUIET)
if(Sofa.GL_FOUND)
    list(APPEND SOFABINDINGS_MODULE_LIST GL)
else()
    message(STATUS "Sofa.GL not found: python bindings Sofa.GL will not be compiled.")
endif()

foreach(sofabindings_module ${SOFABINDINGS_MODULE_LIST})
	add_subdirectory(src/SofaPython3/Sofa/${sofabindings_module})
endforeach()

SP3_add_python_package(
    SOURCE_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}/package
    TARGET_DIRECTORY
        Sofa
)

add_library(${PROJECT_NAME} INTERFACE)
add_library(SofaPython3::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

foreach(sofabindings_module ${SOFABINDINGS_MODULE_LIST})
	target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}.${sofabindings_module})
endforeach()

if(SP3_BUILD_TEST)
    add_subdirectory(tests)
endif()

sofa_create_component_in_package_with_targets(
    COMPONENT_NAME ${PROJECT_NAME}
    COMPONENT_VERSION ${SofaPython3_VERSION}
    PACKAGE_NAME SofaPython3
    TARGETS ${PROJECT_NAME}
    )

project(Bindings.Sofa)

set(SOFABINDINGS_MODULE_LIST
    Lifecycle
    Components
    Core
    Helper
    Simulation
    Types
    )

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

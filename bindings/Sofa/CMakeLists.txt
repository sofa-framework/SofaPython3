project(Bindings.Sofa)

set(SOFABINDINGS_MODULE_LIST Core Components Helper Simulation)

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

install(TARGETS ${PROJECT_NAME} EXPORT BindingsTargets)

if(SP3_BUILD_TEST)
    add_subdirectory(tests)
endif()

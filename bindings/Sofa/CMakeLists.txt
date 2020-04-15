project(Bindings.Sofa)

add_subdirectory(src/SofaPython3/Sofa/Core)
add_subdirectory(src/SofaPython3/Sofa/Components)
add_subdirectory(src/SofaPython3/Sofa/Helper)
add_subdirectory(src/SofaPython3/Sofa/Simulation)
add_subdirectory(src/SofaPython3/Sofa/Types)

SP3_add_python_package(
    SOURCE_DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}/package
    TARGET_DIRECTORY
        ${SP3_PYTHON_PACKAGES_DIRECTORY}/Sofa
)

add_custom_target(${PROJECT_NAME}.ALL)
add_dependencies(${PROJECT_NAME}.ALL ${PROJECT_NAME}.Core)
add_dependencies(${PROJECT_NAME}.ALL ${PROJECT_NAME}.Components)
add_dependencies(${PROJECT_NAME}.ALL ${PROJECT_NAME}.Helper)
add_dependencies(${PROJECT_NAME}.ALL ${PROJECT_NAME}.Simulation)
add_dependencies(${PROJECT_NAME}.ALL ${PROJECT_NAME}.Types)

if(SP3_BUILD_TEST)
    add_subdirectory(tests)
endif()

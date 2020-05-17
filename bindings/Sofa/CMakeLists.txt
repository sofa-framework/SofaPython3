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
        Sofa
)

add_library(${PROJECT_NAME} INTERFACE)
add_library(SofaPython3::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}.Core)
target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}.Components)
target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}.Helper)
target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}.Simulation)
target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}.Types)

install(TARGETS ${PROJECT_NAME} EXPORT BindingsTargets)

if(SP3_BUILD_TEST)
    add_subdirectory(tests)
endif()

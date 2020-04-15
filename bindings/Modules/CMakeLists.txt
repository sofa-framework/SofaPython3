project(Bindings.Modules)

add_subdirectory(src/SofaPython3/SofaBaseTopology)

add_library(${PROJECT_NAME} INTERFACE)
add_library(SofaPython3::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}.SofaBaseTopology)
install(TARGETS ${PROJECT_NAME} EXPORT BindingsTargets)
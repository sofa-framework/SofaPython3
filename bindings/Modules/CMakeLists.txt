project(Bindings.Modules)


add_subdirectory(src/SofaPython3/SofaBaseTopology)

add_custom_target(${PROJECT_NAME}.ALL)
add_dependencies(${PROJECT_NAME}.ALL ${PROJECT_NAME}.SofaBaseTopology)
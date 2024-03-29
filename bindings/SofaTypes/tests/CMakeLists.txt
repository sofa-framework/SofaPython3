project(Bindings.SofaTypes.Tests)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/PythonModule_SofaTypes_test.cpp
)

set(PYTHON_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/pyfiles/vector_test.py
)

find_package(Sofa.Testing REQUIRED)

enable_testing()
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PYTHON_FILES})
target_link_libraries(${PROJECT_NAME} Sofa.Testing SofaPython3Testing)
target_compile_definitions(${PROJECT_NAME} PRIVATE "PYTHON_TESTFILES_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/pyfiles/\"")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Bindings/Tests")

# This will set rpaths relative to all SOFA core AND relocatable dependencies
sofa_auto_set_target_rpath(
    TARGETS ${PROJECT_NAME}
    RELOCATABLE "plugins"
)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

SP3_configure_directory(${CMAKE_CURRENT_SOURCE_DIR}/pyfiles ${CMAKE_BINARY_DIR}/${RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.d/pyfiles)
install(DIRECTORY ${CMAKE_BINARY_DIR}/${RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.d/pyfiles DESTINATION ${RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.d)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT BindingsTargets
    RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY}
)

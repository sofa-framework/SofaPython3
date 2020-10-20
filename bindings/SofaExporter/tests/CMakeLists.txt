project(Bindings.SofaExporter.Tests)

set(SOURCE_FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/PythonModule_SofaExporter_test.cpp
)

set(PYTHON_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/STLExporter.py
  )

find_package(SofaGTestMain REQUIRED)
find_package(SofaFramework REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PYTHON_FILES})
target_link_libraries(${PROJECT_NAME} SofaGTestMain SofaHelper SofaPython3::Plugin SofaPython3::Bindings.SofaExporter)
target_compile_definitions(${PROJECT_NAME} PRIVATE "PYTHON_TESTFILES_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/\"")

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        INSTALL_RPATH_USE_LINK_PATH TRUE
        INSTALL_RPATH "$ORIGIN/../lib"
)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

SP3_configure_directory(${CMAKE_CURRENT_SOURCE_DIR}/tests ${CMAKE_BINARY_DIR}/${RUNTIME_OUTPUT_DIRECTORY}/tests)
install(DIRECTORY ${CMAKE_BINARY_DIR}/${RUNTIME_OUTPUT_DIRECTORY}/tests DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})

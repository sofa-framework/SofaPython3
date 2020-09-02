project(Bindings.SofaExporter.Tests)

set(SOURCE_FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/PythonModule_SofaExporter_test.cpp
)

set(PYTHON_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/test-files/STLExporter.py
  )

find_package(SofaGTestMain REQUIRED)
find_package(SofaFramework REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PYTHON_FILES})
target_link_libraries(${PROJECT_NAME} SofaGTestMain SofaHelper SofaPython3::Plugin SofaPython3::Bindings.SofaExporter)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        INSTALL_RPATH_USE_LINK_PATH TRUE
        INSTALL_RPATH "$ORIGIN/../lib"
)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

SP3_make_link(${CMAKE_CURRENT_SOURCE_DIR}/test-files ${RUNTIME_OUTPUT_DIRECTORY}/tests/SofaExporter)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test-files DESTINATION ${RUNTIME_OUTPUT_DIRECTORY}/tests/SofaExporter)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT BindingsTargets
)

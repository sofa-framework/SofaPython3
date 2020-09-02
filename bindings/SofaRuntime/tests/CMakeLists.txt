project(Bindings.SofaRuntime.Tests)

set(SOURCE_FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/PythonModule_SofaRuntime_test.cpp
)

set(PYTHON_FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/test-files/Base.py
)

find_package(SofaGTestMain REQUIRED)
find_package(SofaFramework REQUIRED)

enable_testing()
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PYTHON_FILES})
target_link_libraries(${PROJECT_NAME} SofaGTestMain SofaHelper SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core)

set(RPATH "$ORIGIN/../lib")
if (APPLE)
    list(APPEND RPATH "@executable_path/../lib")
endif()

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    INSTALL_RPATH_USE_LINK_PATH TRUE
    INSTALL_RPATH "${RPATH}"
)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

SP3_make_link(${CMAKE_CURRENT_SOURCE_DIR}/test-files ${CMAKE_BINARY_DIR}/${RUNTIME_OUTPUT_DIRECTORY}/tests/SofaRuntime)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test-files DESTINATION ${RUNTIME_OUTPUT_DIRECTORY}/tests/SofaRuntime)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT BindingsTargets
)

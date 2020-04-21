project(Bindings.SofaRuntime.Tests)

set(SOURCE_FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/PythonModule_SofaRuntime_test.cpp
)

set(PYTHON_FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/tests/Base.py
)

find_package(SofaGTestMain REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PYTHON_FILES})
target_link_libraries(${PROJECT_NAME} SofaGTestMain SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core)
target_compile_definitions(${PROJECT_NAME} PRIVATE "PYTHON_TESTFILES_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"")

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})


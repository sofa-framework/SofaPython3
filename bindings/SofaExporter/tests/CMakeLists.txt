cmake_minimum_required(VERSION 3.1)
project(PythonModule_SofaExporter_test)

####################################################################################################
### Module dependencies
####################################################################################################
if (NOT TARGET SofaPython3)
  find_package(SofaPython3 QUIET)
  if(NOT SofaPython3_FOUND)
    message("-- PythonModule_Sofa_test is disabled because 'SofaPython3' is missing or not activated.")
    return()
  endif()
endif()

message("-- PythonModule_Sofa_test is enabled.")

####################################################################################################
### Building
####################################################################################################
set(SOURCE_FILES
    PythonModule_SofaExporter_test.cpp
)

set(PYTHON_FILES
    tests/STLExporter.py
    )

find_package(SofaGTestMain REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PYTHON_FILES})
target_link_libraries(${PROJECT_NAME} SofaGTestMain SofaPython3 SofaPython3_Sofa_Core SofaExporter)
target_compile_definitions(${PROJECT_NAME} PRIVATE "PYTHON_TESTFILES_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/\"")

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})


cmake_minimum_required(VERSION 3.1)
project(PythonModule_SofaRuntime_test)

####################################################################################################
### Module dependencies
####################################################################################################
if (NOT TARGET SofaPython3)
  find_package(SofaPython3 QUIET)
  if(NOT SofaPython3_FOUND)
    message("-- PythonModule_SofaRuntime_test is disabled because 'SofaPython3' is missing or not activated.")
    return()
  endif()
endif()

message("-- PythonModule_SofaRuntime_test is enabled.")

find_package(SofaTest QUIET)
if(NOT SofaTest_FOUND)
    message("-- PythonModule_SofaRuntime_test is disabled because 'SofaTest' is missing or not activated.")
    return()
endif()



####################################################################################################
### Building
####################################################################################################
set(SOURCE_FILES
    AllPythonSofaGeometry_test.cpp
)

set(PYTHON_FILES
    pyfiles/Plane_test.py
    pyfiles/Ray_test.py
    pyfiles/vector_test.py
    )

find_package(SofaGTestMain REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PYTHON_FILES})
target_link_libraries(${PROJECT_NAME} SofaTest SofaGTestMain)
target_compile_definitions(${PROJECT_NAME} PRIVATE "PYTHON_TESTFILES_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/pyfiles/\"")

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})


cmake_minimum_required(VERSION 3.1)
project(PythonModule_Sofa_test)

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
set(HEADER_FILES
    Binding_BaseObject_MockComponent.h
)


set(SOURCE_FILES
    PythonModule_Sofa_test.cpp
    Binding_BaseObject_MockComponent.cpp
)



set(PYTHON_FILES
    Components/Components.py
    Core/AutoDiffForceField.py
    Core/BaseData.py
    Core/Base.py
    Core/BaseObject.py
    Core/Controller.py
    Core/ForceField.py
    Core/PythonRestShapeForceField.py
    Helper/Message.py
    Types/RGBAColor.py
    Types/Vec3.py
    Types/BoundingBox.py
    Simulation/Node.py
    bench_datacontainer.py
    bench_node.py
    benchmark.py
    dataaccess.py
    )

find_package(SofaGTestMain REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${PYTHON_FILES})
target_link_libraries(${PROJECT_NAME} SofaGTestMain SofaPython3 SofaPython3_Sofa_Types SofaPython3_Sofa_Simulation SofaPython3_Sofa_Helper SofaPython3_Sofa_Core)
target_compile_definitions(${PROJECT_NAME} PRIVATE "PYTHON_TESTFILES_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/\"")

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})


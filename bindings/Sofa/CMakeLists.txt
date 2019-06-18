cmake_minimum_required(VERSION 3.1)
project(SofaPython3_Sofa VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

####################################################################################################
### Module dependencies
####################################################################################################
if (NOT TARGET SofaPython3)
    find_package(SofaPython3 REQUIRED)
endif()

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Base.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseObject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_DataContainer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseController.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Controller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_ForceField.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Visitor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Node.h

    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/DataCache.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/DataHelper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Submodule_Core.h

    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Helper/Submodule_Helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Helper/Binding_Vector.h

    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Simulation/Submodule_Simulation.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Base.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseController.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_DataContainer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Controller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_ForceField.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Visitor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Node.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/DataCache.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/DataHelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Submodule_Core.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Helper/Submodule_Helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Helper/Binding_Vector.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Simulation/Submodule_Simulation.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Module_Sofa.cpp
)

set(PYTHON_FILES
    package/__Sofa_Types__/__init__.py
    package/__Sofa_Types__/RGBAColor.py
    package/__Sofa_Types__/Vec3.py
    )


sofa_add_pybind11_module(
             TARGET SofaPython3_Sofa
             SOURCES  ${SOURCE_FILES} ${HEADER_FILES} ${PYTHON_FILES}
             DEPENDS SofaCore SofaSimulationGraph SofaPython3 pybind11::module
             OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/package/"
             NAME Sofa
    )

target_include_directories(SofaPython3_Sofa PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src")
target_include_directories(SofaPython3_Sofa PUBLIC "$<INSTALL_INTERFACE:include>")

## Python configuration file (build tree), the lib in the source dir (easier while developping .py files)
file(WRITE "${CMAKE_BINARY_DIR}/etc/sofa/python.d/Sofa" "${CMAKE_CURRENT_SOURCE_DIR}/package/")

## Python configuration file (install tree)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/installed-Sofa-config"
             "modules/Sofa/python/")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/installed-Sofa-config"
         DESTINATION "etc/sofa/python.d"
         RENAME "Sofa")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in"
               "${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}Config.cmake")

if(SOFA_BUILD_TESTS)
    add_subdirectory(tests)
endif()

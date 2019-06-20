cmake_minimum_required(VERSION 3.1)
project(SofaPython3_Sofa VERSION 1.0)

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

    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Simulation/Submodule_Simulation.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Module_Sofa.cpp
)


set(MODULE_NAME Sofa)
set(PACKAGE_DIRECTORY  ${SP3_PYTHON_PACKAGES_DIRECTORY}/Sofa)

SP3_add_python_package(
        SOURCE_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}/package
        TARGET_DIRECTORY
            ${PACKAGE_DIRECTORY}
)

SP3_add_python_module(
    TARGET
        ${PROJECT_NAME}
    MODULE_NAME
        ${MODULE_NAME}
    SOURCES
        ${SOURCE_FILES}
        ${HEADER_FILES}
    DEPENDS
        SofaCore SofaSimulationGraph SofaPython3
    DESTINATION
        ${PACKAGE_DIRECTORY}
)

if(SP3_BUILD_TEST)
    add_subdirectory(tests)
endif()

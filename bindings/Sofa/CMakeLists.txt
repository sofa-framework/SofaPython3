cmake_minimum_required(VERSION 3.1)
project(SofaPython3_Sofa VERSION 1.0)

####################################################################################################
### Module dependencies
####################################################################################################
if (NOT TARGET SofaPython3)
    find_package(SofaPython3 REQUIRED)
endif()

set(CORE_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Base.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Base_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseObject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseObject_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseCamera.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_DataContainer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseController.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Controller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_ForceField.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Visitor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Node.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Node_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Submodule_Core.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/PythonScriptEvent.h
    )

set(TYPES_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Types/Submodule_Types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Types/Binding_BoundingBox.h
    )

set(HELPER_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Helper/Submodule_Helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Helper/Binding_Vector.h
    )

set(HELPER_SYSTEM_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Helper/System/Submodule_System.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Helper/System/Binding_FileRepository.h
    )

set(SIMULATION_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Simulation/Submodule_Simulation.h
    )
set(COMPONENTS_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Components/Submodule_Components.h
    )

set(CORE_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Base.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseCamera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseController.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_DataContainer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Controller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_ForceField.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Visitor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Node.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Submodule_Core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/PythonScriptEvent.cpp
    )

set(TYPES_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Types/Submodule_Types.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Types/Binding_BoundingBox.cpp
    )

set(HELPER_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Helper/Submodule_Helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Helper/Binding_Vector.cpp
    )

set(HELPER_SYSTEM_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Helper/System/Submodule_System.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Helper/System/Binding_FileRepository.cpp
    )

set(SIMULATION_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Simulation/Submodule_Simulation.cpp
    )

set(COMPONENTS_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Components/Submodule_Components.cpp
    )

set(MODULE_NAMES "Core, Simulation, Helper, Types, Components")
set(PACKAGE_DIRECTORY  ${SP3_PYTHON_PACKAGES_DIRECTORY})

SP3_add_python_package(
        SOURCE_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}/package
        TARGET_DIRECTORY
            ${PACKAGE_DIRECTORY}/Sofa
)

SP3_add_python_module(
    TARGET
        ${PROJECT_NAME}_Core
    MODULE_NAME
        Core
    SOURCES
        ${CORE_SOURCE_FILES}
        ${CORE_HEADER_FILES}
    DEPENDS
        SofaCore SofaSimulationCore SofaSimulationGraph SofaPython3 SofaBaseVisual
    DESTINATION
        ${PACKAGE_DIRECTORY}/Sofa
)

SP3_add_python_module(
    TARGET
        ${PROJECT_NAME}_Helper
    MODULE_NAME
        Helper
    SOURCES
        ${HELPER_SOURCE_FILES}
        ${HELPER_HEADER_FILES}
        ${HELPER_SYSTEM_SOURCE_FILES}
        ${HELPER_SYSTEM_HEADER_FILES}
    DEPENDS
        SofaCore SofaSimulationCore SofaSimulationGraph SofaPython3 SofaBaseVisual
    DESTINATION
        ${PACKAGE_DIRECTORY}/Sofa
)

SP3_add_python_module(
    TARGET
        ${PROJECT_NAME}_Simulation
    MODULE_NAME
        Simulation
    SOURCES
        ${SIMULATION_SOURCE_FILES}
        ${SIMULATION_HEADER_FILES}
    DEPENDS
        SofaCore SofaSimulationCore SofaSimulationGraph SofaPython3 SofaBaseVisual
    DESTINATION
        ${PACKAGE_DIRECTORY}/Sofa
)

SP3_add_python_module(
    TARGET
        ${PROJECT_NAME}_Types
    MODULE_NAME
        Types
    SOURCES
        ${TYPES_SOURCE_FILES}
        ${TYPES_HEADER_FILES}
    DEPENDS
        SofaCore SofaSimulationCore SofaSimulationGraph SofaPython3 SofaBaseVisual
    DESTINATION
        ${PACKAGE_DIRECTORY}/Sofa
)

SP3_add_python_module(
    TARGET
        ${PROJECT_NAME}_Components
    MODULE_NAME
        Components
    SOURCES
        ${COMPONENTS_SOURCE_FILES}
        ${COMPONENTS_HEADER_FILES}
    DEPENDS
        SofaCore SofaSimulationCore SofaSimulationGraph SofaPython3 SofaBaseVisual
    DESTINATION
        ${PACKAGE_DIRECTORY}/Sofa
)


if(SP3_BUILD_TEST)
    add_subdirectory(tests)
endif()

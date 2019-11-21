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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_DataDict.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_DataDict_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseObject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseObject_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseCamera.h    
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Controller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Controller_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_DataEngine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_DataEngine_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_ForceField.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_ForceField_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Visitor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Visitor_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Node.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Node_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_NodeIterator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Prefab.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Prefab_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Data/Binding_DataContainer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Data/Binding_DataContainer_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Data/Binding_DataString.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Data/Binding_DataString_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Data/Binding_DataVectorString.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Data/Binding_DataVectorString_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Submodule_Core.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_PythonScriptEvent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseLink.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseLink_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseData_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseCamera_doc.h
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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Simulation/Submodule_Simulation_doc.h
    )
set(COMPONENTS_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Components/Submodule_Components.h
    )

set(CORE_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Base.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_DataDict.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseCamera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Controller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_DataEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Data/Binding_DataContainer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Data/Binding_DataString.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Data/Binding_DataVectorString.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_ForceField.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Visitor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Node.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_NodeIterator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_Prefab.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Submodule_Core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_PythonScriptEvent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Sofa/Core/Binding_BaseLink.cpp
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


add_custom_target(Sofa_Sub_Packages)
add_dependencies(Sofa_Sub_Packages ${PROJECT_NAME}_Components)
add_dependencies(Sofa_Sub_Packages ${PROJECT_NAME}_Types)
add_dependencies(Sofa_Sub_Packages ${PROJECT_NAME}_Core)
add_dependencies(Sofa_Sub_Packages ${PROJECT_NAME}_Simulation)
add_dependencies(Sofa_Sub_Packages ${PROJECT_NAME}_Helper)

if(SP3_BUILD_TEST)
    add_subdirectory(tests)
endif()

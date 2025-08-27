project(Bindings.Sofa.Core)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Base.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Base_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseClass.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseClass_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_DataDict.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_DataDict_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseObject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseObject_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseCamera.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseContext.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_ContactListener.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_ContactListener_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Context.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Controller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Controller_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_DataEngine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_DataEngine_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_ForceField.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_ForceField_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_LinkPath.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_LinkPath_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Mass.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Mass_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_ObjectFactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_ObjectFactory_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Mapping.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Mapping_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Node.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Node_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_NodeIterator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_PointSetTopologyModifier.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Prefab.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Prefab_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Binding_DataContainer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Binding_DataContainer_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Binding_DataString.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Binding_DataLink.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Binding_DataString_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Binding_DataVectorString.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Binding_DataVectorString_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_PythonScriptEvent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseLink.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseLink_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseData_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseCamera_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Topology.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseMeshTopology.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_TaskScheduler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_TaskScheduler_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_VisualParams.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_VisualParams_doc.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Base.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseClass.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_DataDict.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseCamera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_ContactListener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Controller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_DataEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Binding_DataContainer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Binding_DataString.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Binding_DataLink.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Binding_DataVectorString.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_ForceField.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_LinkPath.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Mass.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_ObjectFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Mapping.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Node.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_NodeIterator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_PointSetTopologyModifier.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Prefab.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_PythonScriptEvent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseLink.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Topology.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseMeshTopology.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_TaskScheduler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_VisualParams.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Core.cpp
)

if (NOT TARGET SofaPython3::Plugin)
    sofa_find_package(SofaPython3 REQUIRED COMPONENTS Plugin)
endif()

sofa_find_package(Sofa.Simulation.Core REQUIRED)
sofa_find_package(Sofa.SimpleApi REQUIRED)
sofa_find_package(Sofa.Component.Collision.Response.Contact REQUIRED) # for Binding_ContactListener
sofa_find_package(Sofa.Component.Visual REQUIRED) # for Binding_BaseCamera

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    PACKAGE      Bindings.Sofa
    MODULE       Core
    DESTINATION  Sofa
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      Sofa.Simulation.Core Sofa.SimpleApi Sofa.Component.Collision.Response.Contact Sofa.Component.Visual SofaPython3::Plugin
)

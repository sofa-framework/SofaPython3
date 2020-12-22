project(Bindings.Sofa.Core)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Base.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Base_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_DataDict.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_DataDict_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseObject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseObject_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseCamera.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseContext.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Context.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Controller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Controller_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_DataEngine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_DataEngine_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_ForceField.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_ForceField_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_ObjectFactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_ObjectFactory_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Node.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Node_doc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_NodeIterator.h
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
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Base.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_DataDict.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseCamera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseContext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Controller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_DataEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Binding_DataContainer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Binding_DataString.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Binding_DataLink.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Data/Binding_DataVectorString.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_ForceField.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_ObjectFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Node.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_NodeIterator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Prefab.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_PythonScriptEvent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseLink.cpp
)

if (NOT TARGET SofaPython3::Plugin)
    find_package(SofaPython3 REQUIRED)
endif()

find_package(SofaFramework REQUIRED)
find_package(SofaBase REQUIRED)
find_package(SofaCommon REQUIRED)
find_package(SofaSimulation REQUIRED)

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    PACKAGE_NAME Sofa
    MODULE_NAME  Core
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      SofaBase SofaCore SofaSimulation SofaCommon SofaPython3::Plugin
)

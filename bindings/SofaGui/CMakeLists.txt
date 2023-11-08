cmake_minimum_required(VERSION 3.12)
project(Bindings.SofaGui)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaGui/Module_SofaGui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaGui/Binding_BaseGui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaGui/Binding_GUIManager.cpp
)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaGui/Binding_BaseGui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SofaGui/Binding_GUIManager.h
)

sofa_find_package(Sofa.Core REQUIRED)
sofa_find_package(Sofa.GUI.Common REQUIRED) # to get the GUI mechanism

sofa_find_package(Sofa.GUI.Batch QUIET)
if(Sofa.GUI.Batch_FOUND)
     list(APPEND SUPPORTED_GUIS Sofa.GUI.Batch)
endif()
sofa_find_package(Sofa.GUI.Qt QUIET)
if(Sofa.GUI.Qt_FOUND)
     list(APPEND SUPPORTED_GUIS Sofa.GUI.Qt)
endif()


sofa_find_package(SofaGLFW QUIET)
if(SofaGLFW_FOUND)
     list(APPEND SUPPORTED_GUIS SofaGLFW)
endif()
sofa_find_package(SofaImGui QUIET)
if(SofaImGui_FOUND)
    list(APPEND SUPPORTED_GUIS SofaImGui)
endif()

sofa_find_package(Sofa.GUI.HeadlessRecorder QUIET)
if(Sofa.GUI.HeadlessRecorder_FOUND)
     list(APPEND SUPPORTED_GUIS Sofa.GUI.HeadlessRecorder)
endif()

if(SUPPORTED_GUIS)
     message(STATUS "SofaPython3: Bindings.SofaGui will support: ${SUPPORTED_GUIS} .")
else()
     message(WARNING "SofaPython3: No GUIs detected.")
endif()

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    MODULE       Gui
    DESTINATION  Sofa
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      Sofa.Core Sofa.GUI.Common SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core ${SUPPORTED_GUIS}
)

sofa_create_component_in_package_with_targets(
    COMPONENT_NAME ${PROJECT_NAME}
    COMPONENT_VERSION ${SofaPython3_VERSION}
    PACKAGE_NAME SofaPython3
    TARGETS ${PROJECT_NAME}
    )

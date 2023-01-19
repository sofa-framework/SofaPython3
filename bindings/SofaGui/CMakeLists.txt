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

set(DEPENDS_LIST Sofa.Core Sofa.GL Sofa.GUI.Common SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core)

sofa_find_package(Sofa.Core REQUIRED)
sofa_find_package(Sofa.GL REQUIRED)

sofa_find_package(Sofa.GUI.Common REQUIRED) # to get the GUI mechanism

sofa_find_package(Sofa.GUI.Batch QUIET)
if(Sofa.GUI.Batch_FOUND)
     list(APPEND DEPENDS_LIST Sofa.GUI.Batch)
endif()
sofa_find_package(Sofa.GUI.Qt QUIET)
if(Sofa.GUI.Qt_FOUND)
     list(APPEND DEPENDS_LIST Sofa.GUI.Qt)
endif()
sofa_find_package(Sofa.GUI.HeadlessRecorder QUIET)
if(Sofa.GUI.HeadlessRecorder_FOUND)
     list(APPEND DEPENDS_LIST Sofa.GUI.HeadlessRecorder)
endif()

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    MODULE       Gui
    DESTINATION  Sofa
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      ${DEPENDS_LIST}
)

sofa_create_component_in_package_with_targets(
    COMPONENT_NAME ${PROJECT_NAME}
    COMPONENT_VERSION ${SofaPython3_VERSION}
    PACKAGE_NAME SofaPython3
    TARGETS ${PROJECT_NAME}
    )

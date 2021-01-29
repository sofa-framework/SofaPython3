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

find_package(SofaGui REQUIRED)
find_package(SofaFramework REQUIRED)

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    PACKAGE      Bindings
    MODULE       Gui
    DESTINATION  Sofa
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      SofaCore SofaGuiCommon SofaGui SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core
)

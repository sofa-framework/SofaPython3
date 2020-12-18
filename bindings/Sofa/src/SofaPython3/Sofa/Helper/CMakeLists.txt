project(Bindings.Sofa.Helper)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Vector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/System/Submodule_System.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_MessageHandler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/System/Binding_FileRepository.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_MessageHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_Vector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/Submodule_System.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/System/Binding_FileRepository.cpp
)

find_package(SofaFramework REQUIRED)

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    PACKAGE_NAME Sofa
    MODULE_NAME  Helper
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      SofaCore SofaPython3::Plugin
)

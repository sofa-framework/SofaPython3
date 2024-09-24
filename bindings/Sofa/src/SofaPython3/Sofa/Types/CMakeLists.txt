project(Bindings.Sofa.Types)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BoundingBox.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_CompressedRowSparseMatrix.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Types.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_CompressedRowSparseMatrix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BoundingBox.cpp
)

sofa_find_package(Sofa.Core REQUIRED)

SP3_add_python_module(
        TARGET       ${PROJECT_NAME}
        PACKAGE      Bindings.Sofa
        MODULE       Types
        DESTINATION  Sofa
        SOURCES      ${SOURCE_FILES}
        HEADERS      ${HEADER_FILES}
        DEPENDS      Sofa.Core SofaPython3::Plugin SofaPython3::Bindings.Sofa.Core
)

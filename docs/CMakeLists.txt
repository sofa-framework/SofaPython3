project(Documentation)

set(DOCUMENTATION_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/sphinx/source/index.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/sphinx/source/menu/Installation.rst
    ${CMAKE_CURRENT_SOURCE_DIR}/sphinx/source/menu/Contributing.rst
    ${CMAKE_CURRENT_SOURCE_DIR}/sphinx/source/menu/SofaPlugin.rst
    ${CMAKE_CURRENT_SOURCE_DIR}/sphinx/source/menu/SofaModule.rst
    ${CMAKE_CURRENT_SOURCE_DIR}/sphinx/source/menu/CustomModule.rst
)

add_custom_target(${PROJECT_NAME} SOURCES ${DOCUMENTATION_FILES})
install(FILES ${DOCUMENTATION_FILES} DESTINATION share/SofaPython3/documentation COMPONENT resources)
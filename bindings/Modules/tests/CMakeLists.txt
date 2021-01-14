project(Bindings.Modules.Tests)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(PYTHON_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaDeformable/LinearSpring.py
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaDeformable/SpringForceField.py
)

find_package(SofaGTestMain REQUIRED)
find_package(SofaFramework REQUIRED)

enable_testing()
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PYTHON_FILES})
target_link_libraries(${PROJECT_NAME} SofaGTestMain SofaHelper SofaPython3::Plugin)
target_compile_definitions(${PROJECT_NAME} PRIVATE "PYTHON_TESTFILES_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/\"")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Bindings/Tests")

# This will set rpaths relative to all SOFA core AND relocatable dependencies
sofa_auto_set_target_rpath(
    TARGETS ${PROJECT_NAME}
    RELOCATABLE "plugins"
)

# This will set rpaths relative to SP3 plugin library
get_target_property(RPATH ${PROJECT_NAME} "INSTALL_RPATH")
list(APPEND RPATH "$ORIGIN/../lib")
if (APPLE)
    list(APPEND RPATH "@executable_path/../lib")
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "${RPATH}")

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

set(DIR_BINDING_LIST SofaBaseTopology SofaDeformable)
get_property(_isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
foreach(dir_binding ${DIR_BINDING_LIST})
    if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${dir_binding}")
        continue()
    endif()
    if (_isMultiConfig) # MSVC
        foreach(config_type ${CMAKE_CONFIGURATION_TYPES})
            SP3_configure_directory(${CMAKE_CURRENT_SOURCE_DIR}/${dir_binding} ${CMAKE_BINARY_DIR}/${RUNTIME_OUTPUT_DIRECTORY}/${config_type}/${dir_binding})
            install(DIRECTORY ${CMAKE_BINARY_DIR}/${RUNTIME_OUTPUT_DIRECTORY}/${config_type}/${dir_binding} DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
        endforeach()
    else()
        SP3_configure_directory(${CMAKE_CURRENT_SOURCE_DIR}/${dir_binding} ${CMAKE_BINARY_DIR}/${RUNTIME_OUTPUT_DIRECTORY}/${dir_binding})
        install(DIRECTORY ${CMAKE_BINARY_DIR}/${RUNTIME_OUTPUT_DIRECTORY}/${dir_binding} DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})
    endif()
endforeach()

install(
        TARGETS ${PROJECT_NAME}
        EXPORT BindingsTargets
        RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY}
)

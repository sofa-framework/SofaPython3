project(Bindings.Sofa.Tests)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseObject_MockComponent.h
)

set(SOURCE_FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/PythonModule_Sofa_test.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/Binding_BaseObject_MockComponent.cpp
)

set(PYTHON_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Components/Components.py
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/BaseData.py
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Base.py
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/BaseObject.py
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Controller.py
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/ForceField.py
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/DataEngine.py
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/MyRestShapeForceField.py
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/BaseLink.py
    ${CMAKE_CURRENT_SOURCE_DIR}/Helper/Message.py
    ${CMAKE_CURRENT_SOURCE_DIR}/Types/RGBAColor.py
    ${CMAKE_CURRENT_SOURCE_DIR}/Types/Vec3.py
    ${CMAKE_CURRENT_SOURCE_DIR}/Types/BoundingBox.py
    ${CMAKE_CURRENT_SOURCE_DIR}/Simulation/Node.py
    ${CMAKE_CURRENT_SOURCE_DIR}/Simulation/Simulation.py
    ${CMAKE_CURRENT_SOURCE_DIR}/bench_datacontainer.py
    ${CMAKE_CURRENT_SOURCE_DIR}/bench_node.py
    ${CMAKE_CURRENT_SOURCE_DIR}/benchmark.py
    ${CMAKE_CURRENT_SOURCE_DIR}/dataaccess.py
  )

find_package(SofaGTestMain REQUIRED)
find_package(SofaFramework REQUIRED)

enable_testing()
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${PYTHON_FILES})
target_link_libraries(${PROJECT_NAME} SofaGTestMain Sofa.Helper SofaPython3::Plugin)
target_compile_definitions(${PROJECT_NAME} PRIVATE "PYTHON_TESTFILES_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/\"")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Bindings/Tests")

# This will set rpaths relative to all SOFA core AND relocatable dependencies
sofa_auto_set_target_rpath(
    TARGETS ${PROJECT_NAME}
    RELOCATABLE "plugins"
)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

set(DIR_BINDING_LIST
    Components
    Core
    Helper
    Simulation
    Types
    )
get_property(_isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
foreach(dir_binding ${DIR_BINDING_LIST})
    if (_isMultiConfig) # MSVC
        foreach(config_type ${CMAKE_CONFIGURATION_TYPES})
            SP3_configure_directory(${CMAKE_CURRENT_SOURCE_DIR}/${dir_binding} ${CMAKE_BINARY_DIR}/${RUNTIME_OUTPUT_DIRECTORY}/${config_type}/${PROJECT_NAME}.d/${dir_binding})
            install(DIRECTORY ${CMAKE_BINARY_DIR}/${RUNTIME_OUTPUT_DIRECTORY}/${config_type}/${PROJECT_NAME}.d/${dir_binding} DESTINATION ${RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.d)
        endforeach()
    else()
        SP3_configure_directory(${CMAKE_CURRENT_SOURCE_DIR}/${dir_binding} ${CMAKE_BINARY_DIR}/${RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.d/${dir_binding})
        install(DIRECTORY ${CMAKE_BINARY_DIR}/${RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.d/${dir_binding} DESTINATION ${RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.d)
    endif()
endforeach()

install(
    TARGETS ${PROJECT_NAME}
    EXPORT BindingsTargets
    RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY}
)

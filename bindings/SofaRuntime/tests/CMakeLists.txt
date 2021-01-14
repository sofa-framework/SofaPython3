project(Bindings.SofaRuntime.Tests)

set(SOURCE_FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/PythonModule_SofaRuntime_test.cpp
)

set(PYTHON_FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/SofaRuntime/Base.py
)

find_package(SofaGTestMain REQUIRED)
find_package(SofaFramework REQUIRED)

enable_testing()
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PYTHON_FILES})
target_link_libraries(${PROJECT_NAME} SofaGTestMain SofaHelper SofaPython3::Plugin)
target_compile_definitions(${PROJECT_NAME} PRIVATE "PYTHON_TESTFILES_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"")
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

SP3_configure_directory(${CMAKE_CURRENT_SOURCE_DIR}/SofaRuntime ${CMAKE_BINARY_DIR}/${RUNTIME_OUTPUT_DIRECTORY}/SofaRuntime)
install(DIRECTORY ${CMAKE_BINARY_DIR}/${RUNTIME_OUTPUT_DIRECTORY}/SofaRuntime DESTINATION ${RUNTIME_OUTPUT_DIRECTORY})

install(
    TARGETS ${PROJECT_NAME}
    EXPORT BindingsTargets
    RUNTIME DESTINATION ${RUNTIME_OUTPUT_DIRECTORY}
)

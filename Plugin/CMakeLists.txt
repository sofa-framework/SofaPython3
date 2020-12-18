project(Plugin VERSION 1.0)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/config.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/initModule.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/PythonEnvironment.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SceneLoaderPY3.h

    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/DataCache.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/DataHelper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/PythonFactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Prefab.h
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/initModule.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/PythonEnvironment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/SceneLoaderPY3.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/DataCache.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/DataHelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/PythonFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/Prefab.cpp
)

if(SP3_BUILD_TEST)
    list(APPEND HEADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/PythonTest.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/PythonTestExtractor.h
    )
    list(APPEND SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/PythonTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SofaPython3/PythonTestExtractor.cpp
    )
endif(SP3_BUILD_TEST)

find_package(pybind11 CONFIG REQUIRED)
find_package(SofaFramework REQUIRED)
find_package(SofaSimulation REQUIRED)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
add_library(SofaPython3::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_d")
target_compile_definitions(${PROJECT_NAME} PRIVATE "-DSOFA_BUILD_SOFAPYTHON3")
target_compile_definitions(${PROJECT_NAME} PUBLIC "-DSOFA_HAVE_SOFAPYTHON3")

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<INSTALL_INTERFACE:include>")

target_link_libraries(${PROJECT_NAME} PUBLIC SofaCore SofaSimulationCore SofaSimulationGraph SofaHelper)
target_link_libraries(${PROJECT_NAME} PUBLIC  pybind11::module pybind11::embed)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME SofaPython3)
set_target_properties(${target} PROPERTIES VERSION "${SOFAPYTHON3_VERSION}")

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        INSTALL_RPATH_USE_LINK_PATH TRUE
)

if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    # Sized deallocaion is not enabled by default under clang after c++14
    target_compile_options(${PROJECT_NAME} PUBLIC "-fsized-deallocation")
endif ()

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    # dlopen() is used on Linux for a workaround (see PythonEnvironnement.cpp)
    target_link_libraries(${PROJECT_NAME} PRIVATE dl)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL Windows )
    target_include_directories(${PROJECT_NAME} PUBLIC "${pybind11_INCLUDE_DIR}")

    # https://github.com/boostorg/system/issues/32
    target_compile_definitions(${PROJECT_NAME} PUBLIC "-DHAVE_SNPRINTF")
endif()

# Install header files
foreach(header ${HEADER_FILES})
    file(RELATIVE_PATH relative_path "${CMAKE_CURRENT_SOURCE_DIR}/src" "${header}")
    get_filename_component(relative_directory ${relative_path} DIRECTORY)

    install(FILES "${header}" DESTINATION "include/${relative_directory}" COMPONENT headers)
endforeach()

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/PluginConfig.cmake.in"
        "${CMAKE_BINARY_DIR}/cmake/SofaPython3/PluginConfig.cmake"
        INSTALL_DESTINATION
            "${LIBRARY_OUTPUT_DIRECTORY}/cmake/SofaPython3"
)

install(FILES "${CMAKE_BINARY_DIR}/cmake/SofaPython3/PluginConfig.cmake" DESTINATION "${LIBRARY_OUTPUT_DIRECTORY}/cmake/SofaPython3")

install(TARGETS ${PROJECT_NAME}
    EXPORT PluginTargets
    RUNTIME DESTINATION "${RUNTIME_OUTPUT_DIRECTORY}" COMPONENT applications
    LIBRARY DESTINATION "${LIBRARY_OUTPUT_DIRECTORY}" COMPONENT libraries
    ARCHIVE DESTINATION "${ARCHIVE_OUTPUT_DIRECTORY}" COMPONENT libraries
)

install(
    EXPORT PluginTargets
    NAMESPACE SofaPython3::
    DESTINATION "${LIBRARY_OUTPUT_DIRECTORY}/cmake/SofaPython3"
    COMPONENT headers
)


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
find_package(SofaSimulationGraph REQUIRED)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
add_library(SofaPython3::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PRIVATE "-DSOFA_BUILD_SOFAPYTHON3")

target_link_libraries(${PROJECT_NAME} PUBLIC SofaCore SofaDefaultType SofaSimulationCore SofaSimulationGraph SofaHelper)
target_link_libraries(${PROJECT_NAME} PUBLIC pybind11::module pybind11::embed)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME SofaPython3)

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

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        CXX_VISIBILITY_PRESET "hidden"
        CUDA_VISIBILITY_PRESET "hidden"
)

sofa_create_component_in_package_with_targets(
    COMPONENT_NAME ${PROJECT_NAME}
    COMPONENT_VERSION ${SofaPython3_VERSION}
    PACKAGE_NAME SofaPython3
    TARGETS ${PROJECT_NAME} AUTO_SET_TARGET_PROPERTIES
    INCLUDE_SOURCE_DIR "src"
    INCLUDE_INSTALL_DIR "."
    OPTIMIZE_BUILD_DIR FALSE
    RELOCATABLE ".."
    )

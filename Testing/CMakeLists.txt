project(SofaPython3Testing VERSION 1.0)

find_package(Sofa.Testing REQUIRED)
find_package(Sofa.SimpleApi REQUIRED)

set(HEADER_FILES
    src/SofaPython3Testing/config.h.in
    src/SofaPython3Testing/PythonTest.h
    src/SofaPython3Testing/PythonTestExtractor.h
)

set(SOURCE_FILES
    src/SofaPython3Testing/init.cpp
    src/SofaPython3Testing/PythonTest.cpp
    src/SofaPython3Testing/PythonTestExtractor.cpp
)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC Sofa.Testing SofaPython3::Plugin)
target_compile_definitions(${PROJECT_NAME} PRIVATE "-DSOFA_BUILD_SOFAPYTHON3")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Testing)

sofa_create_package_with_targets(
    PACKAGE_NAME ${PROJECT_NAME}
    PACKAGE_VERSION ${SofaPython3_VERSION}
    TARGETS ${PROJECT_NAME} AUTO_SET_TARGET_PROPERTIES
    INCLUDE_SOURCE_DIR "src"
    INCLUDE_INSTALL_DIR "${PROJECT_NAME}"
)

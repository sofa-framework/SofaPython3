cmake_minimum_required(VERSION 3.1)
project(PythonModule_SofaRuntime)

####################################################################################################
### Module dependencies
####################################################################################################
find_package(SofaPython3 REQUIRED)

####################################################################################################
### Module files
####################################################################################################
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pybind/Module_SofaRuntime.cpp
)


# sofa_add_pybind11_module( TARGET MyModule SOURCES ${SOURCE_FILES} DEPENDS Deps1 Deps2  OUTPUT ${CMAKE_CURRENT_BIN_DIR} NAME python_module_name)
sofa_add_pybind11_module(
        TARGET PythonModule_SofaRuntime
        SOURCES  ${SOURCE_FILES}
        DEPENDS SofaPython3 SofaSimulationGraph SofaPython3_Sofa
        OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/package/"
        NAME SofaRuntime
)

####################################################################################################
### Module packaging
####################################################################################################
## Python configuration file (build tree), the lib in the source dir (easier while developping .py files)
file(WRITE "${CMAKE_BINARY_DIR}/etc/sofa/python.d/SofaRuntime" "${CMAKE_CURRENT_SOURCE_DIR}/package/")

## Python configuration file (install tree)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/installed-SofaRuntime-config"
             "modules/SofaRuntime/python/")


install(FILES "${CMAKE_CURRENT_BINARY_DIR}/installed-SofaRuntime-config"
         DESTINATION "etc/sofa/python.d"
         RENAME "SofaRuntime")

#install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/module/Sofa DESTINATION modules/Sofa/python)

#add_subdirectory(tests)

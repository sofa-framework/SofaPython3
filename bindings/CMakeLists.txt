project(Bindings)

set(BINDINGS_MODULE_LIST Sofa SofaRuntime SofaGui SofaTypes Modules)

foreach(bindings_module ${BINDINGS_MODULE_LIST})
    add_subdirectory(${bindings_module})
endforeach()

if (SP3_WITH_SOFAEXPORTER)
  add_subdirectory(SofaExporter)
endif()

add_library(${PROJECT_NAME} INTERFACE)
add_library(SofaPython3::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

foreach(bindings_module ${BINDINGS_MODULE_LIST})
    target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}.${bindings_module})
endforeach()

if (SP3_WITH_SOFAEXPORTER)
    target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}.SofaExporter)
endif()

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/BindingsConfig.cmake.in"
    "${CMAKE_BINARY_DIR}/cmake/SofaPython3/BindingsConfig.cmake"
    INSTALL_DESTINATION
        "${LIBRARY_OUTPUT_DIRECTORY}/cmake/SofaPython3"
)

install(FILES "${CMAKE_BINARY_DIR}/cmake/SofaPython3/BindingsConfig.cmake" DESTINATION "${LIBRARY_OUTPUT_DIRECTORY}/cmake/SofaPython3")

install(TARGETS ${PROJECT_NAME} EXPORT BindingsTargets)

install(
    EXPORT BindingsTargets
    NAMESPACE SofaPython3::
    DESTINATION "${LIBRARY_OUTPUT_DIRECTORY}/cmake/SofaPython3"
    COMPONENT headers
)

if (SP3_COMPILED_AS_SOFA_SUBPROJECT)
    ## Python configuration file (build tree), the lib in the source dir (easier while developping .py files)
    file(WRITE "${CMAKE_BINARY_DIR}/etc/sofa/python.d/plugin.SofaPython3.bindings" "${CMAKE_BINARY_DIR}/${SP3_PYTHON_PACKAGES_DIRECTORY}\n")

    ## Python configuration file (install tree)
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/installed-bindings-config"
            "${CMAKE_INSTALL_PREFIX}/${SP3_PYTHON_PACKAGES_DIRECTORY}\n")


    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/installed-bindings-config"
            DESTINATION "etc/sofa/python.d"
            RENAME "plugin.SofaPython3.bindings")
endif()
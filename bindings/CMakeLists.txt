project(Bindings)

set(BINDINGS_MODULE_LIST Sofa SofaRuntime SofaGui SofaTypes Modules)
if(SP3_WITH_SOFAEXPORTER)
    list(APPEND BINDINGS_MODULE_LIST SofaExporter)
endif()

# This will set rpaths relative to SP3 plugin library
if(UNIX)
    set(CMAKE_INSTALL_RPATH
        "$ORIGIN/../lib"
        "$$ORIGIN/../lib"
    )
    if(APPLE)
        set(CMAKE_MACOSX_RPATH ON)
        list(APPEND CMAKE_INSTALL_RPATH
            "@loader_path/../lib"
            "@executable_path/../lib"
        )
        set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    endif()
endif()

foreach(bindings_module ${BINDINGS_MODULE_LIST})
    add_subdirectory(${bindings_module})
endforeach()

add_library(${PROJECT_NAME} INTERFACE)
add_library(SofaPython3::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

foreach(bindings_module ${BINDINGS_MODULE_LIST})
    target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}.${bindings_module})
endforeach()

sofa_create_component_in_package_with_targets(
    COMPONENT_NAME ${PROJECT_NAME}
    COMPONENT_VERSION ${SofaPython3_VERSION}
    PACKAGE_NAME SofaPython3
    TARGETS ${PROJECT_NAME}
    )

#if (SP3_COMPILED_AS_SOFA_SUBPROJECT)
#    ## Python configuration file (build tree), the lib in the source dir (easier while developping .py files)
#    file(WRITE "${CMAKE_BINARY_DIR}/etc/sofa/python.d/plugin.SofaPython3.bindings" "${CMAKE_BINARY_DIR}/${SP3_PYTHON_PACKAGES_DIRECTORY}\n")

#    ## Python configuration file (install tree)
#    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/installed-bindings-config"
#            "${CMAKE_INSTALL_PREFIX}/${SP3_PYTHON_PACKAGES_DIRECTORY}\n")


#    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/installed-bindings-config"
#            DESTINATION "etc/sofa/python.d"
#            RENAME "plugin.SofaPython3.bindings")
#endif()

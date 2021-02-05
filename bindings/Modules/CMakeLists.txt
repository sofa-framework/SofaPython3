project(Bindings.Modules)

set(MODULEBINDINGS_MODULE_LIST SofaBaseTopology SofaDeformable)

find_package(Sofa.gl)
if(Sofa.GL_FOUND)
	list(APPEND MODULEBINDINGS_MODULE_LIST SofaGL)
else()
	message("Sofa.GL not detected. Python3 Bindings for Sofa.SofaGL will not be compiled.")
endif()


foreach(modulebindings_module ${MODULEBINDINGS_MODULE_LIST})
	add_subdirectory(src/SofaPython3/${modulebindings_module})
endforeach()


add_library(${PROJECT_NAME} INTERFACE)
add_library(SofaPython3::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

foreach(modulebindings_module ${MODULEBINDINGS_MODULE_LIST})
	target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}.${modulebindings_module})
    set_target_properties(${PROJECT_NAME}.${modulebindings_module} PROPERTIES FOLDER "Bindings/Modules")
endforeach()

install(TARGETS ${PROJECT_NAME} EXPORT BindingsTargets)

if(SP3_BUILD_TEST)
	add_subdirectory(tests)
endif()
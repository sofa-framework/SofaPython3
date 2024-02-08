project(Bindings.Modules)

set(MODULEBINDINGS_MODULE_LIST
    SofaBaseTopology
    SofaDeformable
    SofaLinearSolver
    SofaLinearSystem
    SofaConstraintSolver
)

sofa_find_package(Sofa.GL QUIET)
if(Sofa.GL_FOUND)
	list(APPEND MODULEBINDINGS_MODULE_LIST SofaGL)
else()
	message("Sofa.GL not detected. Python3 Bindings for Sofa.SofaGL will not be compiled.")
endif()


foreach(modulebindings_module ${MODULEBINDINGS_MODULE_LIST})
    message(STATUS "Adding binding module ${modulebindings_module}")
	add_subdirectory(src/SofaPython3/${modulebindings_module})
endforeach()


add_library(${PROJECT_NAME} INTERFACE)
add_library(SofaPython3::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

foreach(modulebindings_module ${MODULEBINDINGS_MODULE_LIST})
	target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}.${modulebindings_module})
    set_target_properties(${PROJECT_NAME}.${modulebindings_module} PROPERTIES FOLDER "Bindings/Modules")
endforeach()

if(SP3_BUILD_TEST)
	add_subdirectory(tests)
endif()

sofa_create_component_in_package_with_targets(
    COMPONENT_NAME ${PROJECT_NAME}
    COMPONENT_VERSION ${SofaPython3_VERSION}
    PACKAGE_NAME SofaPython3
    TARGETS ${PROJECT_NAME}
    )

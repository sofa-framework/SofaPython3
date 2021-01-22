project(Examples)

set(EXAMPLES_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/advanced_timer.py
    ${CMAKE_CURRENT_SOURCE_DIR}/BaseCamera.py
    ${CMAKE_CURRENT_SOURCE_DIR}/basic.py
    ${CMAKE_CURRENT_SOURCE_DIR}/basic-addGUI.py
    ${CMAKE_CURRENT_SOURCE_DIR}/easingScene.py
    ${CMAKE_CURRENT_SOURCE_DIR}/easingSceneMatplotlib.py
    ${CMAKE_CURRENT_SOURCE_DIR}/emptyController.py
    ${CMAKE_CURRENT_SOURCE_DIR}/emptyDataEngine.py
    ${CMAKE_CURRENT_SOURCE_DIR}/emptyForceField.py
    ${CMAKE_CURRENT_SOURCE_DIR}/example-forcefield.py
    ${CMAKE_CURRENT_SOURCE_DIR}/example-scriptcontroller.py
    ${CMAKE_CURRENT_SOURCE_DIR}/keyEvents.py
    ${CMAKE_CURRENT_SOURCE_DIR}/liver.py
    ${CMAKE_CURRENT_SOURCE_DIR}/loadXMLfromPython.py
    ${CMAKE_CURRENT_SOURCE_DIR}/ReadTheDocs_Example.py
    ${CMAKE_CURRENT_SOURCE_DIR}/realTimeClockScene.py
    ${CMAKE_CURRENT_SOURCE_DIR}/pygame_renderingloop/backend_pygame.py
    ${CMAKE_CURRENT_SOURCE_DIR}/pygame_renderingloop/pygame_test.py
    ${CMAKE_CURRENT_SOURCE_DIR}/additional-examples/ControllerScene.py    
)

add_custom_target(${PROJECT_NAME} SOURCES ${EXAMPLES_FILES})
install(FILES ${EXAMPLES_FILES} DESTINATION share/SofaPython3/examples COMPONENT resources)
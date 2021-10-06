project(Bindings.Sofa.Lifetime)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Submodule_Lifetime.cpp
    )

if (NOT TARGET SofaPython3::Plugin)
    find_package(SofaPython3 REQUIRED)
endif()

find_package(Sofa.Config REQUIRED)
find_package(SofaFramework REQUIRED)
find_package(SofaBaseCollision REQUIRED)
find_package(SofaBaseVisual REQUIRED)
find_package(SofaBaseUtils REQUIRED)

SP3_add_python_module(
    TARGET       ${PROJECT_NAME}
    PACKAGE      Bindings
    MODULE       Lifetime
    DESTINATION  Sofa
    SOURCES      ${SOURCE_FILES}
    HEADERS      ${HEADER_FILES}
    DEPENDS      Sofa.Config SofaBaseUtils SofaBaseCollision SofaCore SofaHelper SofaSimulationCore SofaDefaultType SofaBaseVisual SofaPython3::Lifetime SofaPython3::Plugin
)


find_package(pybind11 QUIET)
if(NOT pybind11_FOUND)#
  message("-- Cannot build the python bindings because pybind is missing.")
  return()
endif()

add_subdirectory(Sofa)
add_subdirectory(SofaRuntime)

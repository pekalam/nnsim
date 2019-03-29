cmake_minimum_required (VERSION 3.5)

project(nnsim)
set(CMAKE_SUPPRESS_REGENERATION true)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_subdirectory(matrix-library)
add_subdirectory(nn-library)
add_subdirectory(tests)
add_subdirectory(nnsim-app)

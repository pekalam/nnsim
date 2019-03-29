project (nnsim-tests)


enable_testing()

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()



file(GLOB MX_LIB_SRC "matrix-library-tests/*.cpp")
add_executable(matrix_library_tests ${MX_LIB_SRC})
target_link_libraries(matrix_library_tests gtest_main)
target_link_libraries(matrix_library_tests nnsim::matrix-library)
add_test(NAME matrix_library_tests COMMAND matrix_library_tests)

file(GLOB NN_LIB_SRC "nn-library-tests/*.cpp")
add_executable(nn_library_tests ${NN_LIB_SRC})
target_link_libraries(nn_library_tests gtest_main)
target_link_libraries(nn_library_tests nnsim::nn-library)
add_test(NAME nn_library_tests COMMAND nn_library_tests)
project (matrix-library)

option(ARMADILLO "Use armadillo as matrix-library backend." OFF)
option(ARRAYFIRE "Use arrayfire as matrix-library backend" OFF)

if(ARMADILLO)
	set(ARMADILLO_INCLUDE "" CACHE PATH "Armadillo library include directory")
	add_compile_definitions(MAT_ARMADILLO)
	include_directories(${ARMADILLO_INCLUDE})
endif()
if(ARRAYFIRE)
	set(ARRAYFIRE_INCLUDE "" CACHE DIRECTORY "Arrayfire library include directory")
	add_compile_definitions(MAT_ARRAYFIRE)
	include_directories(${ARRAYFIRE_INCLUDE})
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_SUPPRESS_REGENERATION true)
file(GLOB MXSOURCES "src/*.cpp" "include/*.h")
add_library(${PROJECT_NAME} ${MXSOURCES})

add_library(nnsim::matrix-library ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

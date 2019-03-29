project (nn-library)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_SUPPRESS_REGENERATION true)
file(GLOB NNSOURCES "src/*.cpp" "include/*.h")
add_library(${PROJECT_NAME} ${NNSOURCES})

add_library(nnsim::nn-library ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    nnsim::matrix-library
)
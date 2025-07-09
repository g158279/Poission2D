cmake_minimum_required(VERSION 3.25)

project(Poisson2D LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(EXTERNAL_DIR "F:/guozhongqing/Nuwa_kernel/external/third_party")

set(CMAKE_PREFIX_PATH "${EXTERNAL_DIR}" ${CMAKE_PREFIX_PATH})

find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem program_options)

set(NLOHMANN_JSON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/nlohmann_json/Library/include")
set(GOOGLETEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest-main/googletest-main/googletest/include")

include_directories(
    ${NLOHMANN_JSON_DIR}
)

add_library (Poisson2DLib SHARED)

target_sources (Poisson2DLib PRIVATE 
	Poisson2DLib/mesh.h
	Poisson2DLib/mesh.cpp
	Poisson2DLib/solver.h
	Poisson2DLib/solver.cpp
	Poisson2DLib/utils.h
	Poisson2DLib/utils.cpp
	Poisson2DLib/RunPoisson2D.h
	Poisson2DLib/RunPoisson2D.cpp
)

target_link_libraries(Poisson2DLib PUBLIC
	Eigen3::Eigen
	Boost::boost
)

target_compile_definitions(Poisson2DLib PRIVATE MYLIBRARY_EXPORTS)

option(USE_SPARSE_MATRIX "Use sparse matrix" ON)

if(USE_SPARSE_MATRIX)
    add_definitions(-DUSE_SPARSE_MATRIX)
else()
    add_definitions(-DUSE_DENSE_MATRIX)
endif()

add_executable(Poisson2D
    main.cpp
)

target_link_libraries(Poisson2D PRIVATE Poisson2DLib)

enable_testing()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest-main/googletest-main)

add_executable(Poisson2DLibTest gtest/meshTest.cpp)

target_link_libraries(Poisson2DLibTest PRIVATE Poisson2DLib GTest::gtest GTest::gtest_main)

target_include_directories(Poisson2DLibTest PRIVATE ${CMAKE_SOURCE_DIR}/Poisson2DLib)
target_include_directories(Poisson2DLibTest PRIVATE ${GOOGLETEST_DIR})

add_test(NAME Poisson2DLibTest COMMAND Poisson2DLibTest)

cmake_minimum_required(VERSION 3.14)
project(tests)

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)

set(INSTALL_GTEST OFF)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(tests
	testing)

enable_testing()

include(GoogleTest)

foreach(test ${tests})
	add_executable(${test} ${test}.cpp)
	target_include_directories(${test} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../")
	target_link_libraries(${test} hanjp)
	target_link_libraries(${test} gtest_main)
	add_test(NAME ${test} COMMAND ${test})
	# gtest_discover_tests(${test})
endforeach()

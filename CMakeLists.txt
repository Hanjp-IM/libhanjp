cmake_minimum_required(VERSION 3.14)
project(libhanjp)

set(hangul_USE_STATIC_LIBS OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(hangul libhangul REQUIRED)
pkg_check_modules(GLib REQUIRED glib-2.0)
add_library(hanjp automata.cpp inputcontext.cpp keyboard.cpp)
target_include_directories(hanjp PUBLIC ${hangul_INCLUDE_DIRS})
target_link_libraries(hanjp ${hangul_LIBRARIES})

configure_file(keyboard.xml ${CMAKE_CURRENT_BINARY_DIR}/keyboard.xml COPYONLY)
configure_file(combination.xml ${CMAKE_CURRENT_BINARY_DIR}/combination.xml COPYONLY)

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(testing testing.cpp)
target_include_directories(testing PUBLIC ${GLib_INCLUDE_DIRS})
target_link_libraries(testing PUBLIC hanjp gtest_main ${GLib_LIBRARIES})

include(GoogleTest)
gtest_discover_tests(testing)

install(TARGETS hanjp DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES hanjp.h automata.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
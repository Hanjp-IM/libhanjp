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

add_subdirectory(test EXCLUDE_FROM_ALL)

install(TARGETS hanjp DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES hanjp.h automata.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES keyboard.xml combination.xml DESTINATION ${CMAKE_INSTALL_SHAREDSTATEDIR})
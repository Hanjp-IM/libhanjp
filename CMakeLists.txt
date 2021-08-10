cmake_minimum_required(VERSION 3.14)
project(libhanjp)

set(hangul_USE_STATIC_LIBS OFF)
set(GLib_USE_STATIC_LIBS OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(hangul libhangul REQUIRED)
pkg_check_modules(GLib REQUIRED glib-2.0)
add_library(hanjp hanjpautomata.c hanjpinputcontext.c hanjpkeyboard.c)
target_include_directories(hanjp PUBLIC ${hangul_INCLUDE_DIRS} ${GLib_INCLUDE_DIRS})
target_link_libraries(hanjp ${hangul_LIBRARIES} ${GLib_LIBRARIES})

add_subdirectory(test EXCLUDE_FROM_ALL)
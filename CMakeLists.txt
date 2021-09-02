cmake_minimum_required(VERSION 3.14)
project(libhanjp)

set(hangul_USE_STATIC_LIBS OFF)
set(GObject_USE_STATIC_LIBS OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GObject REQUIRED gobject-2.0)
add_library(hanjp hanjpautomata.c hanjpinputcontext.c hanjpkeyboard.c 
    ${PROJECT_SOURCE_DIR}/libhangul/hangul/hangulctype.c 
    ${PROJECT_SOURCE_DIR}/libhangul/hangul/hangulkeyboard.c
)
target_include_directories(hanjp PUBLIC ${GObject_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/libhangul/hangul)
target_link_libraries(hanjp ${GObject_LIBRARIES})

add_subdirectory(test EXCLUDE_FROM_ALL)
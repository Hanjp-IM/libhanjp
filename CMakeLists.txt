cmake_minimum_required(VERSION 3.14)
project(libhanjp)

set(GObject_USE_STATIC_LIBS OFF)

include(GenerateExportHeader)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GObject REQUIRED gobject-2.0)
add_library(hanjp hanjpautomata.c hanjpinputcontext.c hanjpkeyboard.c 
    ${PROJECT_SOURCE_DIR}/libhangul/hangul/hangulctype.c 
    ${PROJECT_SOURCE_DIR}/libhangul/hangul/hangulkeyboard.c
)
target_include_directories(hanjp PUBLIC ${GObject_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/libhangul/hangul)
target_link_libraries(hanjp ${GObject_LIBRARIES})
generate_export_header(hanjp)

add_subdirectory(test)

install(TARGETS hanjp DESTINATION lib)
install(FILES
    hanjp.h
    hanjpautomata.h
    hanjpkeyboard.h
    hanjpinputcontext.h 
    hanjpunicode.h 
    DESTINATION include
)
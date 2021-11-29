cmake_minimum_required(VERSION 3.14)
project(libhanjp)

set(GObject_USE_STATIC_LIBS OFF)

include(GenerateExportHeader)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GObject REQUIRED gobject-2.0)
pkg_check_modules(Hangul REQUIRED libhangul)
add_library(hanjp hanjpbuffer.c hanjpautomata.c hanjpinputcontext.c hanjpkeyboard.c)
target_include_directories(hanjp PUBLIC ${GObject_INCLUDE_DIRS} PUBLIC ${Hangul_INCLUDE_DIRS})
target_link_libraries(hanjp ${GObject_LIBRARIES} ${Hangul_LIBRARIES})
generate_export_header(hanjp)

add_subdirectory(test)

install(TARGETS hanjp DESTINATION lib)
install(FILES
    hanjp.h
    hanjpautomata.h
    hanjpkeyboard.h
	hanjpbuffer.h
    hanjpinputcontext.h 
    hanjpunicode.h
    DESTINATION include
)

cmake_minimum_required(VERSION 3.10)
project(libhanjp)

include(CTest)

set(hangul_USE_STATIC_LIBS OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(hangul libhangul REQUIRED)
add_library(hanjp STATIC automata.cpp inputcontext.cpp)
target_include_directories(hanjp PUBLIC ${hangul_INCLUDE_DIRS})
target_link_libraries(hanjp ${hangul_LIBRARYS})

include(CheckSymbolExists)
check_symbol_exists(AutomataCustom "automata.h" USE_AM_CUSTOM)
if(USE_AM_CUSTOM)
    target_complie_definitions(hanjp PRIVATE "USE_AM_CUSTOM")
endif()

add_subdirectory(test)

add_test(NAME Test_AM COMMAND test_am)
add_test(NAME Test_IC COMMAND test_ic)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
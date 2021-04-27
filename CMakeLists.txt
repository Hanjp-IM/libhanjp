cmake_minimum_required(VERSION 3.0.0)
project(libhanjp VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(test)

add_library(hangul libhangul/hangul/hangulctype.c libhangul/hangul/hangulkeyboard.c libhangul/hangul/hangulinputcontext.c)
add_library(hanjp automata.cpp inputcontext.cpp)

include(CheckSymbolExists)
check_symbol_exists(AutomataCustom "automata.h" USE_AM_CUSTOM)
if(USE_AM_CUSTOM)
    target_complie_definitions(hanjp PRIVATE "USE_AM_CUSTOM")
endif()

target_include_directories(hanjp PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/libhangul/hangul")
target_link_libraries(hanjp PUBLIC hangul)

add_test(NAME Test_AM COMMAND test_am)
add_test(NAME Test_IC COMMAND test_ic)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
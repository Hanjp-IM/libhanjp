add_executable(test_am test_am.cpp)
add_executable(test_ic test_ic.cpp)

target_link_libraries(test_am PUBLIC hanjp)
target_link_libraries(test_ic PUBLIC hanjp)
target_include_directories(test_am PRIVATE ../)
target_include_directories(test_ic PRIVATE ../)
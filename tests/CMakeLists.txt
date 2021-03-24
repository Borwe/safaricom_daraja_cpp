
# setup catch2 deps
setup_catch2_tester()

add_executable(a_test test.cpp)
target_link_libraries(a_test PRIVATE Catch2::Catch2)
target_include_directories(a_test PRIVATE Catch2::Catch2)

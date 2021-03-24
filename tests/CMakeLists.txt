
# setup catch2 deps
setup_catch2_tester()

enable_testing()

add_executable(token_api token_api.cpp)
target_link_libraries(token_api PRIVATE Catch2::Catch2)
target_include_directories(token_api PRIVATE Catch2::Catch2)

add_test(token_api token_api WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

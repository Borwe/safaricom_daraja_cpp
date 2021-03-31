cmake_minimum_required(VERSION 3.11)

project(Daraja)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


include(DarajaDependencies)
setup_poco_framework()

add_library(Daraja 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tokens/tokens.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tokens/consumer_values.cpp)
add_library(Daraja::Daraja ALIAS Daraja)
target_include_directories(Daraja PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE Poco::Util)
target_link_libraries(Daraja PRIVATE Poco::Util)

add_subdirectory(tests)

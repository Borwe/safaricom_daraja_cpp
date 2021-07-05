#Copyright 2021 Brian Orwe
#
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.

enable_testing()

find_package(Boost 1.75 REQUIRED unit_test_framework filesystem system)

add_executable(token_api token_api.cpp)
target_link_libraries(token_api PRIVATE Daraja::Configuration
    Boost::filesystem
    Boost::system
    Boost::unit_test_framework)

add_test(token_api token_api WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test.properties ${CMAKE_CURRENT_BINARY_DIR})

#add definition to be used for to discover build dir for testfiles
add_compile_definitions(TEST_BUILD_DIR="${CMAKE_CURRENT_BINARY_DIR}")

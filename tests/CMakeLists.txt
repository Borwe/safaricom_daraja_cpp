#Copyright (C) 2021  Brian Orwe
#
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

enable_testing()

find_package(Boost 1.75 REQUIRED unit_test_framework filesystem system)

add_executable(configuration_api configuration_api.cpp)
add_executable(token_api token_api.cpp)

target_link_libraries(configuration_api PRIVATE Daraja::Configuration
    Boost::filesystem
    Boost::system
    Boost::unit_test_framework)
target_link_libraries(token_api PRIVATE Daraja::Tokens
    PRIVATE Daraja::Configuration
    Boost::unit_test_framework)

add_test(configuration_api configuration_api WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test.properties ${CMAKE_CURRENT_BINARY_DIR})

#add definition to be used for to discover build dir for testfiles
add_compile_definitions(TEST_BUILD_DIR="${CMAKE_CURRENT_BINARY_DIR}")

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


cmake_minimum_required(VERSION 3.13)

project(Daraja)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#get boost dependency, minimum version is 1.73
find_package(Boost 1.75 REQUIRED COMPONENTS system filesystem thread)
set(Boost_USE_STATIC_LIBS ON)

add_library(Daraja 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tokens/consumer_values.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tokens/generator.cpp)
add_library(Daraja::Daraja ALIAS Daraja)
target_include_directories(Daraja PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(Daraja PRIVATE Boost::system Boost::filesystem PUBLIC Boost::thread)

add_subdirectory(tests)

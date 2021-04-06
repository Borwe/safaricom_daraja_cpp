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

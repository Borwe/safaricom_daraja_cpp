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

project(daraja)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(BUILD_CONFIGURATION "Build Configuration module" ON)
option(BUILD_TOKENS "Build Tokens module" ON)

if( BUILD_CONFIGURATION )
    message(STATUS "Building Configuration")
    add_subdirectory(Configuration)
endif()

if( BUILD_TOKENS )
    set(BUILD_CONFIGURATION ON)
    message(STATUS "Building Tokens")
    add_subdirectory(Tokens)
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(tests)

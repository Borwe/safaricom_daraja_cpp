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

project(daraja_configuration LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(BoostSetup)

#get boost dependency, minimum version is 1.75
find_package(Boost 1.75 REQUIRED COMPONENTS system filesystem json coroutine)

add_library(daraja_configuration 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/consumer_values.cpp)

add_library(Daraja::Configuration ALIAS daraja_configuration)

include(GenerateExportHeader)
generate_export_header(Daraja::Configuration 
    EXPORT_FILE_NAME 
    ${CMAKE_BINARY_DIR}/exports/configuration/daraja_configuration_export.h)

target_include_directories(daraja_configuration 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC ${CMAKE_BINARY_DIR}/exports/configuration)

target_link_libraries(daraja_configuration 
    PRIVATE Boost::system Boost::filesystem)



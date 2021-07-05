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

project(daraja_tokens LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(BoostSetup)


add_library(daraja_tokens
    ${CMAKE_CURRENT_SOURCE_DIR}/src/generator.cpp)

#get openssl for use with daraja_tokens
find_package(OpenSSL REQUIRED COMPONENTS SSL)
find_package(Threads REQUIRED Threads)
find_package(Boost 1.75 REQUIRED system json coroutine)

target_link_libraries(daraja_tokens 
    PRIVATE Boost::system PRIVATE Daraja::Configuration
    PUBLIC Threads::Threads PRIVATE OpenSSL::SSL 
    PRIVATE Boost::json PRIVATE Boost::coroutine)

add_library(Daraja::Tokens ALIAS daraja_tokens)

include(GenerateExportHeader)
generate_export_header(Daraja::Tokens 
    EXPORT_FILE_NAME
    ${CMAKE_BINARY_DIR}/exports/tokens/daraja_tokens_export.h)

target_include_directories(daraja_tokens
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC ${CMAKE_BINARY_DIR}/exports/tokens/)


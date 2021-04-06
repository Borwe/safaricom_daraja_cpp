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

include(FetchContent)

macro(setup_catch2_tester)
    find_package(Catch2 QUIET)
    if(NOT ${Catch2_FOUND})
        message(STATUS "Didn't find Catch2 on your system, going to try github")
        FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v2.13.4
        )
        FetchContent_GetProperties(Catch2)

        if(NOT Catch2_POPULATED)
            FetchContent_Populate(Catch2)

            add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR})
        endif()
    endif()
endmacro()

macro(setup_poco_framework)
    find_package(Poco QUIET)
    if(NOT ${Poco_FOUND})
        message(STATUS "Didn't find Poco library locally, going to try github")
        FetchContent_Declare(
            Poco
            GIT_REPOSITORY https://github.com/pocoproject/poco.git
            GIT_TAG poco-1.10.1-release
        )
        FetchContent_GetProperties(Poco)

        if(NOT Poco_POPULATED)
            FetchContent_Populate(Poco)
            set(BUILD_SHARED_LIBS_OLD ${BUILD_SHARED_LIBS})
            set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Build shared libs")

            set(ENABLE_FOUNDATION ON)
            set(ENABLE_ENCODINGS ON)
            set(ENABLE_NET ON)
            set(ENABLE_JSON ON)
            set(ENABLE_UTIL ON)
            set(ENABLE_ENCODINGS_COMPILER OFF)
            set(ENABLE_XML OFF)
            set(ENABLE_MONGODB OFF)
            set(ENABLE_DATA_SQLITE OFF)
            set(ENABLE_REDIS OFF)
            set(ENABLE_PDF OFF)
            set(ENABLE_SEVENZIP OFF)
            set(ENABLE_ZIP OFF)
            set(ENABLE_CPPPARSER OFF)
            set(ENABLE_POCODOC OFF)
            set(ENABLE_PAGECOMPILER OFF)
            set(ENABLE_PAGECOMPILER_FILE2PAGE OFF)

            add_subdirectory(${poco_SOURCE_DIR} ${poco_BINARY_DIR})

            set(BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS_OLD} 
                CACHE BOOL "Build shared libs")
            message(STATUS "Poco setup done.")
        endif()
    endif()
endmacro()

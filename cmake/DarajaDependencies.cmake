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

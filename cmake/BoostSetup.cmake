
#set options to be used by user
option(BUILD_SHARED_LIBS "Build Shared library" ON)
if( NOT BUILD_SHARED_LIBS)
    set(Boost_USE_STATIC_LIBS ON)
else()
    set(Boost_USE_STATIC_LIBS OFF)
endif()

cmake_minimum_required( VERSION 2.6 )
project( cgframework-restful )

set( CGFW_DIRECTORY "CGFramework" CACHE STRING "CGFramework location")
include_directories( ${CGFW_DIRECTORY} )
add_subdirectory( ${CGFW_DIRECTORY} )

if( "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra" )
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra" )
endif()

add_executable( test test.cpp )
target_link_libraries( test pixelator )

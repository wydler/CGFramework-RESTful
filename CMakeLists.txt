cmake_minimum_required( VERSION 2.6 )
project( cgframework-restful )

set( CGFW_DIRECTORY "CGFramework" CACHE STRING "CGFramework location")
include_directories( ${CGFW_DIRECTORY} )
add_subdirectory( ${CGFW_DIRECTORY} )

add_executable( test test.cpp )
target_link_libraries( test pixelator )

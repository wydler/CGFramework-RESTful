cmake_minimum_required( VERSION 2.6 )
project( cgframework-restful )

find_package( OpenGL REQUIRED )
include_directories( ${OPENGL_INCLUDE_DIRS} )

set( CGFW_DIRECTORY "CGFramework" CACHE STRING "CGFramework location")

option( USE_INTERNAL_GLFW "Use internal glfw copy instead of system installation" OFF )
if( USE_INTERNAL_GLFW )
    include_directories( "${CGFW_DIRECTORY}/glfw/include/" )
endif()

option( USE_INTERNAL_GLM "Use internal GLM copy instead of system installation" OFF )
if( USE_INTERNAL_GLM )
    include_directories( "${CGFW_DIRECTORY}/glm/" )
endif()

add_subdirectory( ${CGFW_DIRECTORY} )
include_directories( ${CGFW_DIRECTORY} )

add_executable( test test.cpp ${CGFW_DIRECTORY}/Pixelator.cpp ${CGFW_DIRECTORY}/Texture.cpp ${CGFW_DIRECTORY}/BitmapFile.cpp ${CGFW_DIRECTORY}/ImageBuffer.cpp )
target_link_libraries( test glfw ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} )
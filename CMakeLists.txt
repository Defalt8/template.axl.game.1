cmake_minimum_required( VERSION 3.10 )

project( template.axl.game )

set( CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" )

# Sub-module configurations
set( MAIN_TARGET template.axl.game )
set( LIBAXLGAME_DIR "${CMAKE_SOURCE_DIR}/../axl.game" CACHE STRING "axl.game library directory." )
set( LIBAXLGLW_DIR "${CMAKE_SOURCE_DIR}/../axl.glw" CACHE STRING "axl.glw library directory." )
set( LIBAXLMATH_DIR "${CMAKE_SOURCE_DIR}/../axl.math" CACHE STRING "axl.math library directory." )
set( LIBAXLUTIL_DIR "${CMAKE_SOURCE_DIR}/../axl.util" CACHE STRING "axl.util library directory." )

set( LIBAXLUTIL_EXPORT_DIR "${${MAIN_TARGET}_BINARY_DIR}" )
set( LIBAXLMATH_EXPORT_DIR "${${MAIN_TARGET}_BINARY_DIR}" )
set( LIBAXLGLW_EXPORT_DIR "${${MAIN_TARGET}_BINARY_DIR}" )
set( LIBAXLGAME_EXPORT_DIR "${${MAIN_TARGET}_BINARY_DIR}" )

add_subdirectory( "${LIBAXLGAME_DIR}" "axl.game" )
# --------

include_directories( include )

add_executable( ${MAIN_TARGET}
	"src/main.cpp"
	"src/MainView.cpp"
)

target_include_directories( ${MAIN_TARGET}
	PRIVATE "${LIBAXLGAME_DIR}/include"
	PRIVATE "${LIBAXLGLW_DIR}/include"
	PRIVATE "${LIBAXLMATH_DIR}/include"
	PRIVATE "${LIBAXLUTIL_DIR}/include"
)
target_link_libraries( ${MAIN_TARGET}
	PRIVATE axl.game
	PRIVATE axl.glw
	PRIVATE axl.math
	PRIVATE axl.util
	INTERFACE win32.resource
)


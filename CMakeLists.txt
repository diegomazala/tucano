# CMakeLists.txt
#
# Purpose: to compile tucano examples.
# If you want to simply use tucano, you don't need this file; just get the header files.
#
# List of dependencies common to all examples:
#  - glew
#  - eigen3
#  - an OpenGL 4.3+ compatible driver
#  - pkg-config
#
# List of dependencies of some examples:
#  - Qt5
#  - GLFW

cmake_minimum_required(VERSION 2.8.12)
project(tucano C CXX)

# Tucano directories.
#
# Where executables of the examples are put.
set(TUCANO_BINARY_DIR "${PROJECT_SOURCE_DIR}/bin")
set(TUCANO_DOCUMENTATION_DIR "${PROJECT_SOURCE_DIR}/doc")
set(TUCANO_EFFECTS_DIR "${PROJECT_SOURCE_DIR}/effects")
set(TUCANO_SHADERS_DIR "${PROJECT_SOURCE_DIR}/effects/shaders")
set(TUCANO_SAMPLES_DIR "${PROJECT_SOURCE_DIR}/samples")
set(TUCANO_SRC_DIR "${PROJECT_SOURCE_DIR}/src")


set(OPENGL_MAJOR_VERSION 4 CACHE STRING "Opengl version required. Example: 4")
set(OPENGL_MINOR_VERSION 3 CACHE STRING "Opengl version required. Example: 3")

# Build options.
option(SUPPORT_QT_GREATHER_OR_EQUAL_TO_5_4_0 "Compile using Qt version equa or greather than 5.4" ON)
option(DOC "build documentation" OFF)


if (SUPPORT_QT_GREATHER_OR_EQUAL_TO_5_4_0 )
	
	find_package(Qt5OpenGL)
	find_package(Qt5Widgets)
	if (Qt5_POSITION_INDEPENDENT_CODE)
		set(CMAKE_POSITION_INDEPENDENT_CODE ON)
	endif()
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	include_directories(	
		${Qt5Widgets_INCLUDE_DIRS}
		${Qt5OpenGL_INCLUDE_DIRS}
		)
	
	set(USE_QT_GRATHER_OR_EQUAL_TO_5_4_0 1)
		
else (SUPPORT_QT_GREATHER_OR_EQUAL_TO_5_4_0 )

	find_package(GLEW REQUIRED)
	if(NOT GLEW_FOUND)
		message(SEND_ERROR "GLEW not found on your system.")
	endif()

	find_package(OpenGL REQUIRED)
	if(NOT OPENGL_FOUND)
	  message(SEND_ERROR "OpenGL not found on your system.")
	endif()
	
	include_directories(
		${GLEW_INCLUDE_DIRS}
		${OPENGL_INCLUDE_DIR}
		)
	
	set(USE_QT_GRATHER_OR_EQUAL_TO_5_4_0 0)
	
endif (SUPPORT_QT_GREATHER_OR_EQUAL_TO_5_4_0 )


configure_file(config.h.in ${TUCANO_SRC_DIR}/config.h)

# Eigen 3.
# pkg-config is used so we don't hard-code the location of eigen.
find_package(PkgConfig)
if(NOT PKG_CONFIG_FOUND)
    set( EIGEN3_INCLUDE_DIRS "$ENV{EIGEN3_INCLUDE_DIR}" )
    if( NOT EIGEN3_INCLUDE_DIRS )
		message( SEND_ERROR "pkg-config not found on your system. Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
	endif()
else()
    pkg_check_modules(EIGEN3 REQUIRED eigen3)
endif()

include_directories(
  ${EIGEN3_INCLUDE_DIRS}
  ${TUCANO_EFFECTS_DIR}
  ${TUCANO_SRC_DIR}
  )

file(
  GLOB TUCANO_SOURCES
  "${TUCANO_SRC_DIR}/*.hpp"
  )

file(
  GLOB TUCANO_SOURCES_UTILS
  "${TUCANO_SRC_DIR}/utils/*.hpp"
  )

file(
  GLOB TUCANO_SOURCES_SHADERS
  "${TUCANO_SHADERS_DIR}/*.vert"
  "${TUCANO_SHADERS_DIR}/*.tesc"
  "${TUCANO_SHADERS_DIR}/*.tese"
  "${TUCANO_SHADERS_DIR}/*.geom"
  "${TUCANO_SHADERS_DIR}/*.frag"
  )

add_library(
  TUCANO_LIBRARY 
#OBJECT
  ${TUCANO_SOURCES}
  ${TUCANO_SOURCES_UTILS}
  )

install(
  FILES ${TUCANO_SOURCES}
  DESTINATION "include/tucano"
  )

install(
  FILES ${TUCANO_SOURCES_UTILS}
  DESTINATION "include/tucano/utils"
  )

if(DOC)
  add_subdirectory(${TUCANO_DOCUMENTATION_DIR})
endif()





#option(SAMPLES "build samples" OFF)
#if(SAMPLES)
  # Put executable / compiled files in the binary directory.
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TUCANO_BINARY_DIR})
  add_subdirectory(${TUCANO_SAMPLES_DIR})
#endif()

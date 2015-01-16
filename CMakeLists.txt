cmake_minimum_required(VERSION 2.8.12)
set(PROJECT_NAME "tucano")
project(${PROJECT_NAME} C CXX)

# Tucano directories.
set(TUCANO_BINARY_DIR "${PROJECT_SOURCE_DIR}/bin")
set(TUCANO_DOCUMENTATION_DIR "${PROJECT_SOURCE_DIR}/doc")
set(TUCANO_EFFECTS_DIR "${PROJECT_SOURCE_DIR}/effects")
set(TUCANO_SAMPLES_DIR "${PROJECT_SOURCE_DIR}/samples")
set(TUCANO_SRC_DIR "${PROJECT_SOURCE_DIR}/src")

# Build options.
option(DOC "build documentation" ON)
option(SAMPLES "build samples" ON)

find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
  message(SEND_ERROR "GLEW not found on your system.")
endif()

find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
  message(SEND_ERROR "OpenGL not found on your system.")
endif()

# Qt5.
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Widgets REQUIRED)
if (Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# This is for not hard-coding the eigen location.
find_package(PkgConfig REQUIRED)
if(NOT PKG_CONFIG_FOUND)
  message(SEND_ERROR "pkg-config not found on your system")
endif()
pkg_check_modules(EIGEN3 REQUIRED eigen3)

include_directories(
  ${EIGEN3_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIR}
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5OpenGL_INCLUDE_DIRS}
  ${TUCANO_EFFECTS_DIR}
  ${TUCANO_SRC_DIR}
  )

set(
  TUCANO_SOURCES
  ${TUCANO_SRC_DIR}/bufferobject.hpp
  ${TUCANO_SRC_DIR}/camera.hpp
  ${TUCANO_SRC_DIR}/effect.hpp
  ${TUCANO_SRC_DIR}/framebuffer.hpp
  ${TUCANO_SRC_DIR}/mesh.hpp
  ${TUCANO_SRC_DIR}/model.hpp
  ${TUCANO_SRC_DIR}/shader.hpp
  ${TUCANO_SRC_DIR}/texture.hpp
  ${TUCANO_SRC_DIR}/texturemanager.hpp
  ${TUCANO_SRC_DIR}/trackball.hpp
  ${TUCANO_SRC_DIR}/tucano.hpp
  )

set(
  TUCANO_SOURCES_UTILS
  ${TUCANO_SRC_DIR}/utils/misc.hpp
  ${TUCANO_SRC_DIR}/utils/objimporter.hpp
  ${TUCANO_SRC_DIR}/utils/plyimporter.hpp
  ${TUCANO_SRC_DIR}/utils/qtplainwidget.hpp
  ${TUCANO_SRC_DIR}/utils/qttrackballwidget.hpp
  ${TUCANO_SRC_DIR}/utils/rply.hpp
  )

add_library(
  TUCANO_LIBRARY
  OBJECT
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

if(SAMPLES)
  # Put executable / compiled files in the binary directory.
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TUCANO_BINARY_DIR})
  add_subdirectory(${TUCANO_SAMPLES_DIR})
endif()

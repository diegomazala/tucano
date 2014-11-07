cmake_minimum_required(VERSION 2.8.12)
set(PROJECT_NAME "Tucano")
project(${PROJECT_NAME} C CXX)

add_definitions(-DTUCANODEBUG)

set(DOCUMENTATION_DIR "${PROJECT_SOURCE_DIR}/doc")
set(SAMPLES_DIR "${PROJECT_SOURCE_DIR}/samples")
set(TUCANO_EFFECTS_DIR "${PROJECT_SOURCE_DIR}/effects")
set(TUCANO_SRC_DIR "${PROJECT_SOURCE_DIR}/src")

find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
  message(SEND_ERROR "GLEW not found on your system.")
endif()

find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
  message(SEND_ERROR "OpenGL not found on your system.")
endif()

find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Widgets REQUIRED)
if (Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
  ${GLEW_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIR}
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5OpenGL_INCLUDE_DIRS}
  ${TUCANO_EFFECTS_DIR}
  ${TUCANO_SRC_DIR}
  "/usr/include/eigen3"
  )

###########################################
# DOCUMENTATION BEGIN
###########################################

find_package(Doxygen QUIET)
if (DOXYGEN_FOUND)
  add_custom_target(
    doc
    ${DOXYGEN_EXECUTABLE}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Generating ${PROJECT_NAME} documentation with Doxygen."
  )    
  if(EXISTS /usr/bin/git)
  add_custom_target(
    webdoc
    COMMAND git add -A .
    COMMAND git add -u .
    COMMAND git status --short
    COMMAND git commit -m "update web documentation"
    COMMAND git push --force origin gh-pages:gh-pages
    DEPENDS doc
    WORKING_DIRECTORY ${DOCUMENTATION_DIR}/html
    COMMENT "Generating ${PROJECT_NAME} documentation for the web with Doxygen."
    )
  else()
    message(WARNING "git not found on your system.")
  endif()
endif()

###########################################
# DOCUMENTATION END
###########################################

###########################################
# TUCANO BEGIN
###########################################

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
  ${TUCANO_SRC_DIR}/utils/objimporter.hpp
  ${TUCANO_SRC_DIR}/utils/plyimporter.hpp
  ${TUCANO_SRC_DIR}/utils/qtplainwidget.hpp
  ${TUCANO_SRC_DIR}/utils/qttrackballwidget.hpp
  ${TUCANO_SRC_DIR}/utils/rply/rply.c
  ${TUCANO_SRC_DIR}/utils/rply/rply.h
  )

add_library(
  TUCANO_LIBRARY
  OBJECT
  ${TUCANO_SOURCES}
  )

###########################################
# TUCANO END
###########################################

###########################################
# TUCANO_EFFECTS BEGIN
###########################################

set(
  TUCANO_EFFECTS_SOURCES
  ${TUCANO_EFFECTS_DIR}/gradientfilter.hpp
  ${TUCANO_EFFECTS_DIR}/meanfilter.hpp
  ${TUCANO_EFFECTS_DIR}/rendertexture.hpp
  )

add_library(
  TUCANO_EFFECTS_LIBRARY
  OBJECT
  ${TUCANO_EFFECTS_SOURCES}
  )

###########################################
# TUCANO_EFFECTS END
###########################################

# You might comment out the examples you don't want to compile.
add_subdirectory(${SAMPLES_DIR}/imgproc)
add_subdirectory(${SAMPLES_DIR}/phongViewer)
add_subdirectory(${SAMPLES_DIR}/simpleTexture)

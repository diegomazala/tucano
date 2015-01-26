cmake_minimum_required(VERSION 2.8.12)
set(PROJECT_NAME "flyscene")
project(${PROJECT_NAME} C CXX)

set(FLYSCENE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${FLYSCENE_DIR})
include_directories("/usr/local/include/GLFW")

set(
  FLYSCENE_SOURCES
  ${FLYSCENE_DIR}/main.cpp
  )

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
set (GLFW_LIBRARIES ${GLFW_STATIC_LIBRARIES})


add_executable(
  flyscene
  ${FLYSCENE_SOURCES}
  )


target_link_libraries(
  flyscene 
  ${GLEW_LIBRARIES}
  glfw
  ${GLFW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  )

cmake_minimum_required(VERSION 2.8.12)
set(PROJECT_NAME "flyscene")
project(${PROJECT_NAME} C CXX)

set(FLYSCENE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(TUCANO_DIR "../../src/")
set(TUCANO_EFFECTS_DIR "../../effects/")
set(TUCANO_BINARY_DIR "../../../bin/")

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
set (GLFW_LIBRARIES ${GLFW_STATIC_LIBRARIES})

find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
  message(SEND_ERROR "GLEW not found on your system.")
endif()

find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
	message(SEND_ERROR "OpenGL not found on your system.")
endif()


add_definitions(-DTUCANODEBUG)
add_definitions ("-Wall")
add_definitions ("-g")

pkg_check_modules(EIGEN3 REQUIRED eigen3)
include_directories(
	${EIGEN3_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
	${FLYSCENE_DIR}
	${TUCANO_DIR}
	${TUCANO_EFFECTS_DIR}
	)

set(
  FLYSCENE_SOURCES
  ${FLYSCENE_DIR}/main.cpp
  ${FLYSCENE_DIR}/flyscene.cpp
  )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TUCANO_BINARY_DIR})

add_executable(
  flyscene
  ${FLYSCENE_SOURCES}
  )

target_link_libraries(
  flyscene 
  ${GLEW_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  )



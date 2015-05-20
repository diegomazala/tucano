set(FLYSCENE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${FLYSCENE_DIR})

pkg_search_module(GLFW REQUIRED glfw3)
set(GLFW_LIBRARIES ${GLFW_STATIC_LIBRARIES})

set(
  FLYSCENE_SOURCES
  ${FLYSCENE_DIR}/main.cpp
  ${FLYSCENE_DIR}/flyscene.cpp
  )

add_executable(
  flyscene
  ${FLYSCENE_SOURCES}
  ${TUCANO_SOURCES}
  )

set_property(
  TARGET flyscene
  PROPERTY COMPILE_FLAGS
  "-DTUCANOSHADERDIR=${TUCANO_EFFECTS_DIR}/shaders/ -Wall -g"
  )

target_link_libraries(
  flyscene
  ${GLEW_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  )

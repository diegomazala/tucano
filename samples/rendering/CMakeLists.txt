set(RENDERING_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${RENDERING_DIR})

set(
  RENDERING_SOURCES
  ${RENDERING_DIR}/main.cpp
  ${RENDERING_DIR}/mainwindow.cpp
  ${RENDERING_DIR}/glwidget.cpp
  )

qt5_wrap_ui(
  RENDERING_UIS
  ${RENDERING_DIR}/mainwindow.ui
  )

add_executable(
  rendering
  ${RENDERING_SOURCES}
  ${RENDERING_UIS}
  $<TARGET_OBJECTS:TUCANO_LIBRARY>
  )

target_link_libraries(
  rendering
  ${GLEW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  Qt5::OpenGL
  Qt5::Widgets
  )

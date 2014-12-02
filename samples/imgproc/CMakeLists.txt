set(IMGPROC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${IMGPROC_DIR})

set(
  IMGPROC_SOURCES
  ${IMGPROC_DIR}/main.cpp
  ${IMGPROC_DIR}/mainwindow.cpp
  ${IMGPROC_DIR}/glwidget.cpp
  )

qt5_wrap_ui(
  IMGPROC_UIS
  ${IMGPROC_DIR}/mainwindow.ui
  )

add_executable(
  imgproc
  ${IMGPROC_SOURCES}
  ${IMGPROC_UIS}
  $<TARGET_OBJECTS:TUCANO_EFFECTS_LIBRARY>
  $<TARGET_OBJECTS:TUCANO_LIBRARY>
  )

add_dependencies(
  imgproc
  TUCANO_EFFECTS_LIBRARY
  TUCANO_LIBRARY
  )

target_link_libraries(
  imgproc
  ${GLEW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  Qt5::OpenGL
  Qt5::Widgets
  )

set(PHONGVIEWER_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${PHONGVIEWER_DIR})

set(
  PHONGVIEWER_SOURCES
  ${PHONGVIEWER_DIR}/main.cpp
  ${PHONGVIEWER_DIR}/mainwindow.cpp
  ${PHONGVIEWER_DIR}/glwidget.cpp
  )

qt5_wrap_ui(
  PHONGVIEWER_UIS
  ${PHONGVIEWER_DIR}/mainwindow.ui
  )

add_executable(
  phongViewer
  ${PHONGVIEWER_SOURCES}
  ${PHONGVIEWER_UIS}
  $<TARGET_OBJECTS:TUCANO_LIBRARY>
  )

add_dependencies(
  phongViewer
  TUCANO_LIBRARY
  )

target_link_libraries(
  phongViewer
  ${GLEW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  Qt5::OpenGL
  Qt5::Widgets
  )

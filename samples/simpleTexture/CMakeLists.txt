set(SIMPLETEXTURE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${SIMPLETEXTURE_DIR})

set(
  SIMPLETEXTURE_SOURCES
  ${SIMPLETEXTURE_DIR}/main.cpp
  ${SIMPLETEXTURE_DIR}/mainwindow.cpp
  ${SIMPLETEXTURE_DIR}/glwidget.cpp
  )

qt5_wrap_ui(
  SIMPLETEXTURE_UIS
  ${SIMPLETEXTURE_DIR}/mainwindow.ui
  )

add_executable(
  simpleTexture
  ${SIMPLETEXTURE_SOURCES}
  ${SIMPLETEXTURE_UIS}
  $<TARGET_OBJECTS:TUCANO_EFFECTS_LIBRARY>
  $<TARGET_OBJECTS:TUCANO_LIBRARY>
  )

add_dependencies(
  simpleTexture
  TUCANO_EFFECTS_LIBRARY
  TUCANO_LIBRARY
  )

target_link_libraries(
  simpleTexture
  ${GLEW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  Qt5::OpenGL
  Qt5::Widgets
  )

install(
  TARGETS simpleTexture
  RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
  )

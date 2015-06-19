#######################################################################
# Setting Target_Name as current folder name
get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)



set  (SOURCE_FILES		GLWidget.cpp
						main.cpp )

set  (HEADER_FILES		GLWidget.h)



set	 (RESOURCES    qt5-simpleTexture.qrc)
qt5_add_resources(RESOURCE_FILES ${RESOURCES})
source_group ("Resource Files" FILES ${RESOURCES})


include_directories (${CMAKE_SOURCE_DIR})


add_executable(
  ${TARGET_NAME}
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${RESOURCE_FILES}
  $<TARGET_OBJECTS:TUCANO_LIBRARY>
)

target_link_libraries (	
	${TARGET_NAME} 
	Qt5::OpenGL
	Qt5::Widgets
)




#######################################################################
# Setting Target_Name as current folder name
get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)



set  (SOURCE_FILES	helloglfw.cpp)

set  (HEADER_FILES)

source_group("Tucano" FILES ${TUCANO_SOURCES})

	
	

add_executable(
  ${TARGET_NAME}
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${TUCANO_SOURCES}
)



target_link_libraries (	
	${TARGET_NAME} 
	${OPENGL_LIBRARY} 
	${GLEW_LIBRARY}
	${GLFW_LIBRARIES}
)






#######################################################################
# Setting Target_Name as current folder name
get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)



set  (SOURCE_FILES		main.cpp
						GlutWindow.cpp)

set  (HEADER_FILES		GlutApp.h
						GlutWindow.h)



#######################################################################
############################# SETUP GLUT ##############################
find_package(GLUT REQUIRED)
include_directories	(${GLUT_INCLUDE_DIR})
if (NOT EXISTS ${GLUT_LIBRARY_DIR})
	get_filename_component(GLUT_LIBRARY_DIR ${GLUT_LIBRARY} PATH [CACHE])
endif()
link_directories(${GLUT_LIBRARY_DIR})
#######################################################################

#######################################################################
########################## SETUP FREE_IMAGE ###########################
set (FREEIMAGE_INCLUDE_DIR "NOT-FOUND" CACHE PATH "FreeImage include directory")
set (FREEIMAGE_LIBRARY "NOT-FOUND" CACHE FILEPATH "FreeImage library")
get_filename_component(FREE_LIBRARY_DIR ${FREEIMAGE_LIBRARY} PATH [CACHE])
include_directories	(${FREEIMAGE_INCLUDE_DIR})
link_directories	(${FREEIMAGE_LIBRARY_DIR})
#######################################################################
							

add_executable(
  ${TARGET_NAME}
  ${SOURCE_FILES}
  ${HEADER_FILES}
  #$<TARGET_OBJECTS:TUCANO_LIBRARY>
)

target_link_libraries (	
	${TARGET_NAME} 
	${OPENGL_LIBRARY} 
	${GLEW_LIBRARY}
	${GLUT_LIBRARY}
	${FREEIMAGE_LIBRARY}
)






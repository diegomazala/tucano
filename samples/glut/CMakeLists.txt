add_definitions(-DTUCANODEBUG)

#######################################################################
############################# SETUP GLUT ##############################
find_package(GLUT REQUIRED)
include_directories	(${GLUT_INCLUDE_DIR})
if (NOT EXISTS ${GLUT_LIBRARY_DIR})
	get_filename_component(GLUT_LIBRARY_DIR ${GLUT_LIBRARY} PATH [CACHE])
endif()
link_directories(${GLUT_LIBRARY_DIR})
#######################################################################



# Tip: you can comment out the examples you don't want to compile.
add_subdirectory(simpleTexture)

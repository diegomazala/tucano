add_definitions(-DTUCANODEBUG)

# Tip: you can comment out the examples you don't want to compile.
#add_subdirectory(imgproc)
#add_subdirectory(phongViewer)
#add_subdirectory(rendering)
#add_subdirectory(simpleTexture)




if (SUPPORT_QT_GREATHER_OR_EQUAL_TO_5_4_0 )

	option(QT5_SAMPLES "Build Qt5 samples" ON)
	
else (SUPPORT_QT_GREATHER_OR_EQUAL_TO_5_4_0 )


	option(GLUT_SAMPLES "Glut Samples" ON)
	option(GLFW_SAMPLES "GLFW Samples" ON)
		
endif(SUPPORT_QT_GREATHER_OR_EQUAL_TO_5_4_0 )


if (QT5_SAMPLES)
	add_subdirectory(qt5)
endif (QT5_SAMPLES)

if (GLUT_SAMPLES)
		add_subdirectory(glut)
	endif(GLUT_SAMPLES)
		
if (GLFW_SAMPLES)
	add_subdirectory(glfw)
endif(GLFW_SAMPLES)
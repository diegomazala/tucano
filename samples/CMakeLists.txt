add_definitions(-DTUCANODEBUG)

# Tip: you can comment out the examples you don't want to compile.
#add_subdirectory(imgproc)
#add_subdirectory(phongViewer)
#add_subdirectory(rendering)
#add_subdirectory(simpleTexture)

add_subdirectory(qt5-simpleTexture)

option(SAMPLES_GLFW "GLFW Samples" OFF)

if (${SAMPLES_GLFW})
	add_subdirectory(flyscene)
endif()

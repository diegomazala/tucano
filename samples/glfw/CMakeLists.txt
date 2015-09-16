add_definitions(-DTUCANODEBUG)


if( WIN32 ) # true if windows (32 and 64 bit)
	set (GLFW_INCLUDE_DIR "NOT-FOUND" CACHE PATH "glfw include directory")
	set (GLFW_LIBRARY_DIR "NOT-FOUND" CACHE PATH "glfw library directory")
	include_directories	(${GLFW_INCLUDE_DIR})
	link_directories	(${GLFW_LIBRARY_DIR})
	set(GLFW_LIBRARIES glfw3 glfw3dll)
else()
	pkg_search_module(GLFW REQUIRED glfw3)
	set(GLFW_LIBRARIES ${GLFW_STATIC_LIBRARIES})
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TUCANO_BINARY_DIR}/glfw)

# Tip: you can comment out the examples you don't want to compile.
add_subdirectory(helloglfw)


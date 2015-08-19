#######################################################################
# Setting Target_Name as current folder name
get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)


set (SOURCE_FILES	
					mainwindow.cpp
					glwidget.cpp 
					main.cpp 
	)
	
set (HEADER_FILES	
					mainwindow.h
					glwidget.h
					TerrainMesh.hpp
	)

set (SHADER_FILES	
					${TUCANO_SHADERS_DIR}/terrain.vert
					${TUCANO_SHADERS_DIR}/terrain.tesc
					${TUCANO_SHADERS_DIR}/terrain.tese
					${TUCANO_SHADERS_DIR}/terrain.geom
					${TUCANO_SHADERS_DIR}/terrain.frag
	)

set (FORM_FILES		mainwindow.ui 
	)


set	 (RESOURCES    terrainTesselation.qrc
	)

qt5_add_resources(RESOURCE_FILES ${RESOURCES})
source_group ("Resource Files" FILES ${RESOURCES})

qt5_wrap_ui( HEADER_UI_FILES ${FORM_FILES} )	
source_group ("Form Files" FILES ${FORM_FILES})
source_group ("Form Header Files" FILES ${HEADER_UI_FILES})

source_group("Tucano" FILES ${TUCANO_SOURCES})
source_group("Tucano Utils" FILES ${TUCANO_SOURCES_UTILS})
source_group("Shaders" FILES ${SHADER_FILES})

include_directories (${CMAKE_SOURCE_DIR})	
	
add_executable(
  ${TARGET_NAME}
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${HEADER_UI_FILES}
  ${RESOURCE_FILES}
  ${SHADER_FILES}
)

target_link_libraries (	
	${TARGET_NAME} 
	TUCANO_LIBRARY
	Qt5::OpenGL
	Qt5::Widgets
)
	

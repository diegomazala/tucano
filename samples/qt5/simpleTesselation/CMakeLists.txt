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
	)

set (SHADER_FILES	
					${TUCANO_SHADERS_DIR}/simpleTess.vert
					${TUCANO_SHADERS_DIR}/simpleTess.tesc
					${TUCANO_SHADERS_DIR}/simpleTess.tese
					${TUCANO_SHADERS_DIR}/simpleTess.geom
					${TUCANO_SHADERS_DIR}/simpleTess.frag
	)


set (FORM_FILES		mainwindow.ui 
	)


set	 (RESOURCES    
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
  ${TUCANO_SOURCES}
  ${TUCANO_SOURCES_UTILS}
  ${SHADER_FILES}
)

target_link_libraries (	
	${TARGET_NAME} 
	Qt5::OpenGL
	Qt5::Widgets
)
	

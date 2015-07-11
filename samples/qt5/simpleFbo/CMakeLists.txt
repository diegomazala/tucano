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

set (FORM_FILES		mainwindow.ui 
	)


qt5_wrap_ui( HEADER_UI_FILES ${FORM_FILES} )	
source_group ("Form Files" FILES ${FORM_FILES})
source_group ("Form Header Files" FILES ${HEADER_UI_FILES})

source_group("Tucano" FILES ${TUCANO_SOURCES})
source_group("Tucano Utils" FILES ${TUCANO_SOURCES_UTILS})

include_directories (${CMAKE_SOURCE_DIR})	
	
add_executable(
  ${TARGET_NAME}
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${HEADER_UI_FILES}
  ${TUCANO_SOURCES}
  ${TUCANO_SOURCES_UTILS}
)

target_link_libraries (	
	${TARGET_NAME} 
	Qt5::OpenGL
	Qt5::Widgets
)
	

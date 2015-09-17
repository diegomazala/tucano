#######################################################################
# Setting Target_Name as current folder name
get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set  (SOURCE_FILES    
      main.cpp
      flyscene.cpp)

set  (HEADER_FILES  
      flyscene.hpp)

source_group("Tucano" FILES ${TUCANO_SOURCES})
source_group("Tucano Utils" FILES ${TUCANO_SOURCES_UTILS})

add_executable(
  ${TARGET_NAME}
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${TUCANO_SOURCES}
  ${TUCANO_SOURCES_UTILS}
)

target_link_libraries ( 
  ${TARGET_NAME} 
  ${OPENGL_LIBRARY} 
  ${GLEW_LIBRARY}
  ${GLFW_LIBRARIES}
)

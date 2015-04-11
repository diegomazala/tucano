find_package(Doxygen QUIET)
if (DOXYGEN_FOUND)
  add_custom_target(
    doc
    ${DOXYGEN_EXECUTABLE}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Generating ${PROJECT_NAME} local documentation with Doxygen"
  )    
  find_file(GIT_EXECUTABLE git)
  if(EXISTS ${GIT_EXECUTABLE})
    add_custom_target(
      webdoc
      COMMAND ${GIT_EXECUTABLE} add -A .
      COMMAND ${GIT_EXECUTABLE} add -u .
      COMMAND ${GIT_EXECUTABLE} status --short
      COMMAND ${GIT_EXECUTABLE} commit -m "update web documentation"
      COMMAND ${GIT_EXECUTABLE} push --force origin gh-pages:gh-pages
      DEPENDS doc
      WORKING_DIRECTORY "${TUCANO_DOCUMENTATION_DIR}/html"
      COMMENT "Generating and updating ${PROJECT_NAME} web documentation with Doxygen"
      )
  else()
    message(WARNING "git not found on your system")
  endif()
else()
    message(WARNING "doxygen not found on your system")
endif()

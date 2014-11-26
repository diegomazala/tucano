add_definitions(-DTUCANODEBUG)

set(
  TUCANO_EFFECTS_SOURCES
  ${TUCANO_EFFECTS_DIR}/gradientfilter.hpp
  ${TUCANO_EFFECTS_DIR}/meanfilter.hpp
  ${TUCANO_EFFECTS_DIR}/rendertexture.hpp
  )

add_library(
  TUCANO_EFFECTS_LIBRARY
  OBJECT
  ${TUCANO_EFFECTS_SOURCES}
  )

# Tip: you can comment out the examples you don't want to compile.
add_subdirectory(imgproc)
add_subdirectory(phongViewer)
add_subdirectory(simpleTexture)

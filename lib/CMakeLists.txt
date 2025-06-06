file(GLOB_RECURSE PMODEL_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

file(GLOB_RECURSE PMODEL_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_library(pmodel STATIC
    ${PMODEL_SOURCES}
    ${PMODEL_HEADERS}    # (optional in a static lib, but helps IDEs index them)
)

target_include_directories(pmodel
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:include>    # only matters if you later install
)

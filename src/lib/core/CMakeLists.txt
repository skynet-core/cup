cmake_minimum_required(VERSION 3.31)
get_filename_component(TARGET_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

project(${TARGET_NAME} VERSION 0.1.0 LANGUAGES C CXX)

include(ExportHelper)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB PUB_MOD_SRC "*.ixx")
file(GLOB PRIV_MOD_SRC "*.cxx")

add_library(${PROJECT_NAME} SHARED)
set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES VERSION ${PROJECT_VERSION}
             SOVERSION ${PROJECT_VERSION})

target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET CXX_MODULES FILES
      ${PUB_MOD_SRC}
  )
target_sources(${PROJECT_NAME}
  PRIVATE
    ${PRIV_MOD_SRC}
)

install(TARGETS ${PROJECT_NAME})

add_exported_target(${PROJECT_NAME} SUPPORTED_COMPONENTS)

cmake_minimum_required(VERSION 3.31)

get_filename_component(TARGET_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(
  ${TARGET_NAME}
  VERSION 0.1.0
  LANGUAGES C CXX)

file(GLOB SRC "*.cxx")

find_package(CLI11 CONFIG REQUIRED)
add_executable(${PROJECT_NAME} ${SRC})
set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES CXX_STANDARD 23
             CXX_STANDARD_REQUIRED ON
             CXX_EXTENSIONS OFF
             INSTALL_RPATH "\$ORIGIN/lib:\$ORIGIN/lib64"
             SKIP_BUILD_RPATH OFF
             BUILD_WITH_INSTALL_RPATH OFF
             INSTALL_RPATH_USE_LINK_PATH
             $<IF:$<BOOL:${DEPOT_STANDALONE_PACKAGE}>,OFF,ON>)
target_link_libraries(${PROJECT_NAME} PRIVATE core CLI11::CLI11)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)

cmake_minimum_required(VERSION 3.31)

get_filename_component(TARGET_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${TARGET_NAME} VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SRC "*.cxx")

find_package(CLI11 CONFIG REQUIRED)
add_executable(${PROJECT_NAME} ${SRC})

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES VERSION ${PROJECT_VERSION})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    core CLI11::CLI11)

install(TARGETS ${PROJECT_NAME})

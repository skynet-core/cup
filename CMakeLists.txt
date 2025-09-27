cmake_minimum_required(VERSION 3.31)

project(depot VERSION 1.0.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

# see
# https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES
     "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()



add_subdirectory(src)


include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/${CMAKE_PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME})

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
  VERSION "${CMAKE_PROJECT_VERSION}"
  COMPATIBILITY AnyNewerVersion)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/${CMAKE_PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME})
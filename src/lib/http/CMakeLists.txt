cmake_minimum_required(VERSION 3.31)
get_filename_component(TARGET_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

project(
  ${TARGET_NAME}
  VERSION 0.1.0
  LANGUAGES C CXX)

include(ExportHelper)

file(GLOB PUB_MOD_SRC "*.ixx")
file(GLOB MOD_SRC "*.cxx")
find_package(Threads REQUIRED)
find_package(Boost CONFIG REQUIRED COMPONENTS headers url)
find_package(OpenSSL CONFIG REQUIRED)

add_library(${PROJECT_NAME} SHARED)

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES VERSION ${PROJECT_VERSION}
             SOVERSION ${PROJECT_VERSION}
             CXX_STANDARD 23
             CXX_STANDARD_REQUIRED ON
             CXX_EXTENSIONS OFF
             INSTALL_RPATH "\$ORIGIN"
             SKIP_BUILD_RPATH OFF
             BUILD_WITH_INSTALL_RPATH
             $<IF:$<BOOL:${DEPOT_STANDALONE_PACKAGE}>,OFF,ON>)

target_sources(${PROJECT_NAME} PUBLIC FILE_SET CXX_MODULES FILES ${PUB_MOD_SRC})
target_sources(${PROJECT_NAME} PRIVATE ${MOD_SRC})
install(TARGETS ${PROJECT_NAME})


target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads Boost::headers Boost::url OpenSSL::SSL OpenSSL::Crypto)
add_exported_target(${PROJECT_NAME} SUPPORTED_COMPONENTS)
if(BUILD_TESTING AND CUP_BUILD_TESTING)
    add_subdirectory(tests)
endif()

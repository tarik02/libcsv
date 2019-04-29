cmake_minimum_required(VERSION 3.10.0)
project(libcsv
  VERSION 0.0.1
  LANGUAGES C
)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

##############################################
# Create target and set properties

add_library(libcsv
  src/libcsv.c
)

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(LibCSV::LibCSV ALIAS libcsv)

#Set target properties
target_include_directories(libcsv
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(libcsv
  PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wunused -Werror>
)

##############################################
# Installation instructions

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/LibCSV)

install(TARGETS libcsv
  EXPORT libcsv-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

#This is required so that the exported target has the name LibCSV and not libcsv
set_target_properties(libcsv PROPERTIES EXPORT_NAME LibCSV)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#Export the targets to a script
install(EXPORT libcsv-targets
  FILE
    LibCSVTargets.cmake
  NAMESPACE
    LibCSV::
  DESTINATION
    ${INSTALL_CONFIGDIR}
)

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/LibCSVConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/LibCSVConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/LibCSVConfig.cmake
  INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

#Install the config, configversion and custom find modules
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/LibCSVConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/LibCSVConfigVersion.cmake
  DESTINATION ${INSTALL_CONFIGDIR}
)

##############################################
## Exporting from the build tree
export(
  EXPORT libcsv-targets
  FILE ${CMAKE_CURRENT_BINARY_DIR}/LibCSVTargets.cmake
  NAMESPACE LibCSV::
)

#Register package in user's package registry
export(PACKAGE LibCSV)


option(BUILD_TESTS "Build all tests." OFF)

if(${BUILD_TESTS})
  include(CTest)
  add_subdirectory(test)
endif()

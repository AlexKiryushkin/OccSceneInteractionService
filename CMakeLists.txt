# -------------------------------------------------------------------------------------------------
# CMake version
# -------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.23 FATAL_ERROR)


# -------------------------------------------------------------------------------------------------
# Project setup
# -------------------------------------------------------------------------------------------------
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)

project(OccSceneInteractionService LANGUAGES CXX)


# -------------------------------------------------------------------------------------------------
# Language requirements
# -------------------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# -------------------------------------------------------------------------------------------------
# Build options
# -------------------------------------------------------------------------------------------------
set(OSIS_BUILD_TESTS OFF CACHE BOOL "Build tests")


# -------------------------------------------------------------------------------------------------
# Additional includes
# -------------------------------------------------------------------------------------------------
include(GenerateExportHeader)
include(GNUInstallDirs)


# -------------------------------------------------------------------------------------------------
# Add project(s)
# -------------------------------------------------------------------------------------------------
add_subdirectory(src)

if (OSIS_BUILD_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()


# -------------------------------------------------------------------------------------------------
# Package Library
# -------------------------------------------------------------------------------------------------
set(CPACK_PACKAGE_NAME OSIS)

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

set(CPACK_VERBATIM_VARIABLES true)

set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)

if(WIN32)
    set(CPACK_GENERATOR ZIP NSIS)
elseif(APPLE)
    set(CPACK_GENERATOR TGZ productbuild)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR TGZ RPM)
else()
    set(CPACK_GENERATOR TGZ)
endif()


# -------------------------------------------------------------------------------------------------
# Add packaging functionality
# -------------------------------------------------------------------------------------------------
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	include(CPack)
endif()

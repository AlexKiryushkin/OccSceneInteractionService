# -------------------------------------------------------------------------------------------------
# Fetch Gtest over github
# -------------------------------------------------------------------------------------------------
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
include(FetchContent)
FetchContent_Declare(googletest 
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)


# -------------------------------------------------------------------------------------------------
# Configure build of googletest
# -------------------------------------------------------------------------------------------------
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK ON CACHE BOOL "" FORCE)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()


# -------------------------------------------------------------------------------------------------
# Include google test so that it can be used by other projects
# -------------------------------------------------------------------------------------------------
include(GoogleTest)


# -------------------------------------------------------------------------------------------------
# Adding subdirectories with tests
# -------------------------------------------------------------------------------------------------
add_subdirectory(TestsOccSceneInteractionService)

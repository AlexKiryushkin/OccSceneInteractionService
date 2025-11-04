# -------------------------------------------------------------------------------------------------
# Set Test Executable
# -------------------------------------------------------------------------------------------------
project(TestsOccSceneInteractionService)


# -------------------------------------------------------------------------------------------------
# Define Test Executable Name
# -------------------------------------------------------------------------------------------------
set(TEST_EXECUTABLE_NAME TestsOccSceneInteractionService)


# -------------------------------------------------------------------------------------------------
# Add Executable
# -------------------------------------------------------------------------------------------------
add_executable(${TEST_EXECUTABLE_NAME})


# -------------------------------------------------------------------------------------------------
# Add Executable Sources
# -------------------------------------------------------------------------------------------------
target_sources(${TEST_EXECUTABLE_NAME} 
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/TestsOccSceneInteractionService/TestViewControllerBase.h)

target_sources(${TEST_EXECUTABLE_NAME} 
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/TestUiRenderSyncObject.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/TestViewControllerBase.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/TestViewControllerMouseZoom.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/TestViewControllerPan.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/TestViewControllerRotation.cpp)


# -------------------------------------------------------------------------------------------------
# Find Packages
# -------------------------------------------------------------------------------------------------
find_package(GTest CONFIG REQUIRED)
find_package(OpenCASCADE REQUIRED COMPONENTS ${OpenCASCADE_Visualization_LIBRARIES})
include(GoogleTest)


# -------------------------------------------------------------------------------------------------
# Link Libraries
# -------------------------------------------------------------------------------------------------
target_link_libraries(
    ${TEST_EXECUTABLE_NAME}
        PRIVATE
            osis::OccSceneInteractionService
            GTest::gtest_main
            GTest::gmock_main
            TKOpenGl
)


# -------------------------------------------------------------------------------------------------
# Include Paths
# -------------------------------------------------------------------------------------------------
target_include_directories(
    ${TEST_EXECUTABLE_NAME}
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/include
            ${CMAKE_SOURCE_DIR}/src/OccSceneInteractionService/include
)


# -------------------------------------------------------------------------------------------------
# Compile definitions
# -------------------------------------------------------------------------------------------------
target_compile_definitions(
    ${TEST_EXECUTABLE_NAME} PUBLIC
        GTEST_LINKED_AS_SHARED_LIBRARY
    )


# -------------------------------------------------------------------------------------------------
# Make Tests Available for CTEST
# -------------------------------------------------------------------------------------------------
gtest_discover_tests(${TEST_EXECUTABLE_NAME}
                     DISCOVERY_TIMEOUT 15)

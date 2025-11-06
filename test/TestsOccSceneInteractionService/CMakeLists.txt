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
        ${CMAKE_CURRENT_LIST_DIR}/include/TestsOccSceneInteractionService/MockCameraListener.h
        ${CMAKE_CURRENT_LIST_DIR}/include/TestsOccSceneInteractionService/TestViewControllerBase.h)

target_sources(${TEST_EXECUTABLE_NAME} 
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/TestUiRenderSyncObject.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/TestViewControllerBase.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/TestViewControllerMouseZoom.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/TestViewControllerPan.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/TestViewControllerRotation.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/TestViewControllerUpdateMouseClick.cpp)


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
# Target properties
# -------------------------------------------------------------------------------------------------
set_target_properties(${TEST_EXECUTABLE_NAME} PROPERTIES CXX_STANDARD 17)


# -------------------------------------------------------------------------------------------------
# Compile definitions
# -------------------------------------------------------------------------------------------------
get_target_property(gtest_targetType GTest::gtest_main TYPE)
if(TARGET_TYPE STREQUAL "SHARED_LIBRARY")
    target_compile_definitions(
        ${TEST_EXECUTABLE_NAME} PRIVATE
            GTEST_LINKED_AS_SHARED_LIBRARY
        )
endif()


# -------------------------------------------------------------------------------------------------
# Make Tests Available for CTEST
# -------------------------------------------------------------------------------------------------
gtest_discover_tests(${TEST_EXECUTABLE_NAME}
                     DISCOVERY_TIMEOUT 15)

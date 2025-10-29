# -------------------------------------------------------------------------------------------------
# Define Library Project
# -------------------------------------------------------------------------------------------------
project(OccSceneInteractionService)


# -------------------------------------------------------------------------------------------------
# Define Library Name
# -------------------------------------------------------------------------------------------------
set(LIBRARY_NAME OccSceneInteractionService)


# -------------------------------------------------------------------------------------------------
# Add Library
# -------------------------------------------------------------------------------------------------
add_library(${LIBRARY_NAME})


# -------------------------------------------------------------------------------------------------
# Add Library Sources
# -------------------------------------------------------------------------------------------------
target_sources(${LIBRARY_NAME} 
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include/OccSceneInteractionService/UiRenderSyncObject.h
        ${CMAKE_CURRENT_LIST_DIR}/include/OccSceneInteractionService/ViewController.h)

target_sources(${LIBRARY_NAME} 
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/ViewController.cpp)


# -------------------------------------------------------------------------------------------------
# Generate Export Header
# -------------------------------------------------------------------------------------------------
generate_export_header(
    ${LIBRARY_NAME}
    EXPORT_FILE_NAME include/OccSceneInteractionService/${LIBRARY_NAME}Exports.h
    EXPORT_MACRO_NAME OSIS_API
)


# -------------------------------------------------------------------------------------------------
# Find Packages
# -------------------------------------------------------------------------------------------------
find_package(OpenCASCADE REQUIRED COMPONENTS ${OpenCASCADE_Visualization_LIBRARIES})


# -------------------------------------------------------------------------------------------------
# Link Libraries
# -------------------------------------------------------------------------------------------------
target_link_libraries(
    ${PROJECT_NAME}
        PUBLIC
            ${OpenCASCADE_Visualization_LIBRARIES}
)


# -------------------------------------------------------------------------------------------------
# Include Paths
# -------------------------------------------------------------------------------------------------
target_include_directories(
    ${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${OpenCASCADE_INCLUDE_DIR}
)

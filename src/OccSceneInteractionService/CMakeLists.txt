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
add_library(osis::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})


# -------------------------------------------------------------------------------------------------
# Add Library Sources
# -------------------------------------------------------------------------------------------------
set(PUBLIC_HEADER_SET "osis_public_headers")
target_sources(${LIBRARY_NAME} 
    PRIVATE
        FILE_SET ${PUBLIC_HEADER_SET}
        TYPE HEADERS
        FILES ${CMAKE_CURRENT_LIST_DIR}/include/OccSceneInteractionService/ICameraListener.h
              ${CMAKE_CURRENT_LIST_DIR}/include/OccSceneInteractionService/IKeyHandler.h
              ${CMAKE_CURRENT_LIST_DIR}/include/OccSceneInteractionService/IMouseClickHandler.h
              ${CMAKE_CURRENT_LIST_DIR}/include/OccSceneInteractionService/IMouseHoverListener.h
              ${CMAKE_CURRENT_LIST_DIR}/include/OccSceneInteractionService/IOwnerHoverListener.h
              ${CMAKE_CURRENT_LIST_DIR}/include/OccSceneInteractionService/KeyboardListener.h
              ${CMAKE_CURRENT_LIST_DIR}/include/OccSceneInteractionService/UiRenderSyncObject.h
              ${CMAKE_CURRENT_LIST_DIR}/include/OccSceneInteractionService/ViewController.h
)

target_sources(${LIBRARY_NAME} 
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/ViewController.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/KeyboardListener.cpp)


# -------------------------------------------------------------------------------------------------
# Generate Export Header
# -------------------------------------------------------------------------------------------------
set(EXPORTED_HEADER_FILEPATH include/OccSceneInteractionService/${LIBRARY_NAME}Exports.h)
generate_export_header(
    ${LIBRARY_NAME}
    EXPORT_FILE_NAME ${EXPORTED_HEADER_FILEPATH}
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
    ${LIBRARY_NAME}
        PUBLIC
            TKV3d
)


# -------------------------------------------------------------------------------------------------
# Include Paths
# -------------------------------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PUBLIC
        ${OpenCASCADE_INCLUDE_DIR}
)


# -------------------------------------------------------------------------------------------------
# Target properties
# -------------------------------------------------------------------------------------------------
set_target_properties(${TEST_EXECUTABLE_NAME} PROPERTIES CXX_STANDARD 17)


# -------------------------------------------------------------------------------------------------
# Install Library
# -------------------------------------------------------------------------------------------------
get_target_property(OSIS_INSTALLED_HEADERS ${LIBRARY_NAME} HEADER_SET_${PUBLIC_HEADER_SET})
list(APPEND OSIS_INSTALLED_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${EXPORTED_HEADER_FILEPATH})
set_target_properties(${LIBRARY_NAME} PROPERTIES PUBLIC_HEADER "${OSIS_INSTALLED_HEADERS}")
message(STATUS ${OSIS_INSTALLED_HEADERS})

install(TARGETS ${LIBRARY_NAME}
    EXPORT Osis
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/OccSceneInteractionService
)

install(EXPORT Osis
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
    NAMESPACE osis::
)

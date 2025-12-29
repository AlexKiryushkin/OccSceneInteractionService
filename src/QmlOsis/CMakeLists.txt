# -------------------------------------------------------------------------------------------------
# Define Library Project
# -------------------------------------------------------------------------------------------------
project(QmlOsis)


# -------------------------------------------------------------------------------------------------
# Define Library Name
# -------------------------------------------------------------------------------------------------
set(LIBRARY_NAME QmlOsis)


# -------------------------------------------------------------------------------------------------
# Add Library
# -------------------------------------------------------------------------------------------------
add_library(${LIBRARY_NAME})
add_library(osis::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})


# -------------------------------------------------------------------------------------------------
# Add Library Sources
# -------------------------------------------------------------------------------------------------
set(PUBLIC_HEADER_SET "qml_osis_public_headers")
target_sources(${LIBRARY_NAME} 
    PRIVATE
        FILE_SET ${PUBLIC_HEADER_SET}
        TYPE HEADERS
        FILES ${CMAKE_CURRENT_LIST_DIR}/include/QmlOsis/QmlOsisView.h
)

target_sources(${LIBRARY_NAME} 
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/QmlOsisView.cpp)


# -------------------------------------------------------------------------------------------------
# Generate Export Header
# -------------------------------------------------------------------------------------------------
set(EXPORTED_HEADER_FILEPATH include/QmlOsis/${LIBRARY_NAME}Exports.h)
generate_export_header(
    ${LIBRARY_NAME}
    EXPORT_FILE_NAME ${EXPORTED_HEADER_FILEPATH}
    EXPORT_MACRO_NAME QML_OSIS_API
)


# -------------------------------------------------------------------------------------------------
# Find Packages
# -------------------------------------------------------------------------------------------------
find_package(OpenCASCADE REQUIRED)
find_package(Qt6 COMPONENTS Core Gui Quick OpenGL QuickControls2 REQUIRED)


# -------------------------------------------------------------------------------------------------
# Link Libraries
# -------------------------------------------------------------------------------------------------
target_link_libraries(${LIBRARY_NAME} 
        PUBLIC
            Qt6::Core
            Qt6::Gui
            Qt6::Quick
            Qt6::OpenGL
            Qt6::QuickControls2
            osis::OccSceneInteractionService
            ${OpenCASCADE_LIBRARIES}
)


# -------------------------------------------------------------------------------------------------
# Include Paths
# -------------------------------------------------------------------------------------------------
target_include_directories(${LIBRARY_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        ${OpenCASCADE_INCLUDE_DIR}
)


# -------------------------------------------------------------------------------------------------
# Compile definitions
# -------------------------------------------------------------------------------------------------
target_compile_definitions(
    ${LIBRARY_NAME} PRIVATE
        OCCT_HANDLE_NOCAST
)


# -------------------------------------------------------------------------------------------------
# Target properties
# -------------------------------------------------------------------------------------------------
set_target_properties(${LIBRARY_NAME}
    PROPERTIES
        CXX_STANDARD 17
        AUTOMOC ON
        AUTORCC OFF
        AUTOUIC OFF
)

# -------------------------------------------------------------------------------------------------
# Install Library
# -------------------------------------------------------------------------------------------------
get_target_property(QML_OSIS_INSTALLED_HEADERS ${LIBRARY_NAME} HEADER_SET_${PUBLIC_HEADER_SET})
list(APPEND QML_OSIS_INSTALLED_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${EXPORTED_HEADER_FILEPATH})
set_target_properties(${LIBRARY_NAME} PROPERTIES PUBLIC_HEADER "${QML_OSIS_INSTALLED_HEADERS}")
message(STATUS ${QML_OSIS_INSTALLED_HEADERS})

install(TARGETS ${LIBRARY_NAME}
    EXPORT QmlOsis
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QmlOsis
)

install(EXPORT QmlOsis
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
    NAMESPACE osis::
)

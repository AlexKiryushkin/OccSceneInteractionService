# -------------------------------------------------------------------------------------------------
# Define Library Project
# -------------------------------------------------------------------------------------------------
project(SimpleQmlOsisScene)


# -------------------------------------------------------------------------------------------------
# Define Library Name
# -------------------------------------------------------------------------------------------------
set(EXECUTABLE_NAME SimpleQmlOsisScene)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(RESOURCES "${CMAKE_CURRENT_LIST_DIR}/src/qml.qrc")

# -------------------------------------------------------------------------------------------------
# Add Library
# -------------------------------------------------------------------------------------------------
# add_qml_dir("${CMAKE_CURRENT_LIST_DIR}")

set(SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp)

# -------------------------------------------------------------------------------------------------
# Project Definition
# -------------------------------------------------------------------------------------------------
qt_add_executable(${EXECUTABLE_NAME} ${SOURCES} ${RESOURCES})


# -------------------------------------------------------------------------------------------------
# Find Packages
# -------------------------------------------------------------------------------------------------
find_package(OpenCASCADE REQUIRED COMPONENTS ${OpenCASCADE_Visualization_LIBRARIES})
find_package(Qt6 COMPONENTS Core Core5Compat Gui Quick OpenGL Multimedia Svg Widgets QuickControls2 REQUIRED)


# -------------------------------------------------------------------------------------------------
# Link Libraries
# -------------------------------------------------------------------------------------------------
target_link_libraries(
    ${EXECUTABLE_NAME}
        PUBLIC
            Qt6::Core
            Qt6::Core5Compat
            Qt6::Gui
            Qt6::Quick
            Qt6::OpenGL
            Qt6::Multimedia
            Qt6::Svg
            Qt6::Widgets
            Qt6::QuickControls2
            osis::QmlOsis
)


# -------------------------------------------------------------------------------------------------
# Include Paths
# -------------------------------------------------------------------------------------------------
target_include_directories(${EXECUTABLE_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        ${OpenCASCADE_INCLUDE_DIR}
)


# -------------------------------------------------------------------------------------------------
# Compile definitions
# -------------------------------------------------------------------------------------------------
target_compile_definitions(
    ${EXECUTABLE_NAME} PRIVATE
        OCCT_HANDLE_NOCAST
)


# -------------------------------------------------------------------------------------------------
# Target properties
# -------------------------------------------------------------------------------------------------
set_target_properties(${TEST_EXECUTABLE_NAME} PROPERTIES CXX_STANDARD 17)

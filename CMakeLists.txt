cmake_minimum_required(VERSION 3.21)
project(QtClangWrapper)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# MANUAL SETTING OF QT:
set(QT_INSTALL_PATH D:/DEPS/QT/6.6.2/mingw_64)
set(Qt6_DIR ${QT_INSTALL_PATH}/lib/cmake/Qt6)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
set(QT_LIBS Qt6::Core Qt6::Gui Qt6::Widgets)

include_directories(include)

file(GLOB_RECURSE HEADER_FILES "include/*.hpp")
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

add_executable(QtClangWrapper ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(QtClangWrapper ${QT_LIBS})

# Configure QT6:

if (WIN32)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()

    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()

    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()

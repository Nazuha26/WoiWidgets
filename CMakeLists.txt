cmake_minimum_required(VERSION 3.30)
project(WoiWidgets)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.9.0/mingw_64")

find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    Svg
    REQUIRED)

add_executable(WoiWidgets main.cpp
        src/core/ResizePreview.cpp
        src/core/ResizePreview.h
        src/widgets/WoiTitleBar.cpp
        src/widgets/WoiTitleBar.h
        src/widgets/WoiPushButton.cpp
        src/widgets/WoiPushButton.h
        src/core/WoiMacros.h
        src/core/WoiWindowFrame.cpp
        src/core/WoiWindowFrame.h
        src/core/Icons.cpp
        src/core/Icons.h
        src/widgets/WoiTitleBarButton.cpp
        src/widgets/WoiTitleBarButton.h
        src/core/ShadowOverlayWindow.cpp
        src/core/ShadowOverlayWindow.h
        src/core/style/WoiThemeEngine.cpp
        src/core/style/WoiThemeEngine.h
        src/core/style/WoiThemeWidget.cpp
        src/core/style/WoiThemeWidget.h
)

# =============== RESOURCES ===============

# ========== ICONS:
qt_add_resources(WoiWidgets "Icons"
    PREFIX "/icons"
    BASE src/resources/icons
    FILES
        src/resources/icons/test.svg
        src/resources/icons/collapse.svg
        src/resources/icons/expand.svg
        src/resources/icons/minimize.svg
        src/resources/icons/maximize.svg
        src/resources/icons/restore.svg
        src/resources/icons/close.svg
)

# ========== FONTS:
qt_add_resources(WoiWidgets "RobotoFonts"
    PREFIX "/roboto"
    BASE src/resources/fonts/roboto
    FILES
        src/resources/fonts/roboto/Roboto-Regular.ttf
        src/resources/fonts/roboto/Roboto-Italic.ttf
        src/resources/fonts/roboto/Roboto-Bold.ttf
        src/resources/fonts/roboto/Roboto-BoldItalic.ttf
)

qt_add_resources(WoiWidgets "UbuntuFonts"
        PREFIX "/ubuntu"
        BASE src/resources/fonts/ubuntu
        FILES
            src/resources/fonts/ubuntu/Ubuntu-Regular.ttf
            src/resources/fonts/ubuntu/Ubuntu-Italic.ttf
            src/resources/fonts/ubuntu/Ubuntu-Bold.ttf
            src/resources/fonts/ubuntu/Ubuntu-BoldItalic.ttf
)
# =============================================

target_link_libraries(WoiWidgets
  Qt::Core
  Qt::Gui
  Qt::Widgets
  Qt::Svg
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if(NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if(NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif()
    endif()
    if(EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif()
    foreach(QT_LIB Core Gui Widgets )
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach(QT_LIB)
endif()

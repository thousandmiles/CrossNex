cmake_minimum_required(VERSION 3.5)

project(CrossNex VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)


set(PROJECT_SOURCES
        main.cpp
        dataservice.h
        dataservice.cpp
        webservice.h
        webservice.cpp
        testcontrol.h
        testcontrol.cpp
        mainwindow.h
        mainwindow.cpp
        navigationtree.h
        navigationtree.cpp
        customtreewidgetitem.h
        customtreewidgetitem.cpp
        newinstancedialog.h
        newinstancedialog.cpp

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CrossNex
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        mainwindow.ui
        images.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CrossNex APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CrossNex SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CrossNex
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(CrossNex PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(CrossNex PRIVATE Qt6::Network)

target_link_libraries(CrossNex PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
)

qt_add_qml_module(CrossNex
    URI Charts
    NO_RESOURCE_TARGET_PATH
    DEPENDENCIES QtQuick
    SOURCES
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CrossNex)
endif()

set_target_properties(CrossNex PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CrossNex
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CrossNex)
endif()

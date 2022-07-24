cmake_minimum_required(VERSION 3.5)

project(QssCreator VERSION 0.1.0.0 LANGUAGES CXX)

configure_file(version.h.in versino.h)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/forms)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test)

add_subdirectory(tests)

set(PROJECT_SOURCES
        src/main.cpp

        src/mainwindow.cpp
        src/mainwindow.h
        forms/mainwindow.ui

        src/widgetspreview.cpp
        src/widgetspreview.h
        forms/widgetspreview.ui

        src/iqssdeditor.h
        src/iqssdeditor.cpp

        src/qssdeditor.h
        src/qssdeditor.cpp

        src/iqssdfileoperations.h
        src/iqssdfileoperations.cpp

        src/qssdfileoperations.cpp
        src/qssdfileoperations.h

        src/idocumentfile.h
        src/idocumentfile.cpp

        src/documentfile.h

        src/findreplacedialog.h
        src/findreplacedialog.cpp
        forms/findreplacedialog.ui

        src/iuserdialogs.h
        src/iuserdialogs.cpp

        src/userdialogs.h

        src/qssdvariable.h
        src/qssdvariable.cpp

        src/iqssdprocessor.h
        src/iqssdprocessor.cpp

        src/qssdprocessor.h
        src/qssdprocessor.cpp

        src/iqssdvariablesmodel.h
        src/iqssdvariablesmodel.cpp

        src/qssdvariablesmodel.h
        src/qssdvariablesmodel.cpp

        src/qssdvariableitemdelegate.h
        src/qssdvariableitemdelegate.cpp
)

qt_add_resources(PROJECT_SOURCES resources/res.qrc)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QssCreator
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QssCreator APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QssCreator SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QssCreator
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    src/
    ${PROJECT_BINARY_DIR}
)
target_link_libraries(QssCreator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(QssCreator PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QssCreator)
endif()


# Creating the installers/packages using cmake scripts
if (CPACK_IFW_ROOT OR DEFINED ENV{QTIFWDIR})
    if (DEFINED ENV{QTDIR})
        if (WIN32)
            include(win-package.cmake)
        elseif (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
            include(rpm-package.cmake)
        endif()
    else()
        message("\nSet environment variable QTDIR to be able to create a package.")
        message("\nQTDIR the current kit folder of Qt installation path, for example C:\Qt\5.15.2\msvc2019_64.")
    endif()
else()
    message("\nIf you want to enable target package you can:")
    message("\t* Either pass -DCPACK_IFW_ROOT=<path> to cmake")
    message("\t* Or set the environment variable QTIFWDIR")
    message("To specify the location of the QtIFW tool suite.")
    message("The specified path should not contain bin at the end.\n")
endif()

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

# Install rule using IFW (Qt Installer Framework)
install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION .
    COMPONENT "${PROJECT_NAME}Installer"
)

# Function that set common properties required for all IFW packages
function(cpack_ifw_common)
    set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}Installer")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Install Qss Creator Application")
    set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})
    set(CPACK_COMPONENTS_ALL "${PROJECT_NAME}Installer")
    set(CPACK_GENERATOR IFW)

    set(CPACK_IFW_PACKAGE_START_MENU_DIRECTORY Qt)
    set(CPACK_IFW_PACKAGE_WIZARD_DEFAULT_WIDTH 700)
    set(CPACK_IFW_PACKAGE_WIZARD_DEFAULT_HEIGHT 550)
    set(CPACK_IFW_PACKAGE_RUN_PROGRAM "@TargetDir@/${PROJECT_NAME}.exe")
    set(CPACK_IFW_VERBOSE ON)

    include(CPack REQUIRED)
    include(CPackIFW REQUIRED)

    cpack_add_component("${PROJECT_NAME}Installer"
        DISPALY_NAME "Qss Creator Installer"
        DESCRIPTION "Install Qss Creator"
        REQUIRED
    )

    cpack_ifw_configure_component("${PROJECT_NAME}Installer"
        ESSENTIAL
        DEFAULT_TRUE
        NAME "oss.alirezah.qsscreator"
        VERSION "${PROJECT_VERSION}"
        RELEASE_DATE "2022-07-24"
        LICENSES "MIT LECENSE" "${CMAKE_SOURCE_DIR}/LICENSE"
    )
    install(FILES ${CMAKE_SOURCE_DIR}/LICENSE
        DESTINATION "."
        COMPONENT "${PROJECT_NAME}Installer"
    )

endfunction()

if (CPACK_IFW_ROOT OR DEFINED ENV{QTIFWDIR})
    if (DEFINED ENV{QTDIR})
        if (WIN32)
            if (EXISTS "$ENV{QTDIR}/bin/windeployqt.exe")
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/bin
                        COMMAND $ENV{QTDIR}/bin/windeployqt.exe --dir ${CMAKE_BINARY_DIR}/bin $<TARGET_FILE:${PROJECT_NAME}>
                )
                file(GLOB binContent ${CMAKE_BINARY_DIR}/bin/*
                    LIST_DIERCTOIES true)
                foreach(item ${binContent})
                    if (IS_DIRECTORY ${item})
                        install(
                            DIRECTORY ${item}
                            DESTINATION .
                            COMPONENT "${PROJECT_NAME}Installer"
                        )
                    else()
                        install(
                            FILES ${item}
                            DESTINATION .
                            COMPONENT "${PROJECT_NAME}Installer"
                        )
                    endif()
                endforeach()
                cpack_ifw_common()
            else()
                message("Unable to find QTDIR/bin/windeployqt.exe")
            endif()
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

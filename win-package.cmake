install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION .
    COMPONENT "${PROJECT_NAME}Installer"
)

include(InstallRequiredSystemLibraries)

# Install rule using IFW (Qt Installer Framework)

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

    set(CPACK_GENERATOR IFW)

    set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}Installer")
    set(CPACK_PACKAGE_VENDOR "Alireza Hs")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Install Qss Creator Application")
    set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})
    SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
    set(CPACK_COMPONENTS_ALL "${PROJECT_NAME}Installer")
    set(CMAKE_PROJECT_HOMEPAGE_URL "https://github.com/alirezah95/QssCreator")

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
else()
    message("Unable to find QTDIR/bin/windeployqt.exe")
endif()

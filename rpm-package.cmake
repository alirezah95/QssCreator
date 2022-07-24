install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    COMPONENT "${PROJECT_NAME}Package"
)

include(InstallRequiredSystemLibraries)


set(CPACK_GENERATOR RPM)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "Alireza Hs")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Install Qss Creator Application")
set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_COMPONENTS_ALL "${PROJECT_NAME}Installer")
set(CMAKE_PROJECT_HOMEPAGE_URL "https://github.com/alirezah95/QssCreator")

execute_process(COMMAND uname -m
    OUTPUT_VARIABLE ARCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message("ARCH: ${ARCH}")
string(TOLOWER ${PROJECT_NAME} PACK_NAME)
set(PACK_NAME "${PACK_NAME}-${CPACK_PACKAGE_VERSION}-${ARCH}")
set(CPACK_PACKAGE_FILE_NAME "${PACK_NAME}")

#    string(TOUPPER ${PROJECT_NAME} MAIN_COMPONENT)

set(CPACK_RPM_PACKAGE_REQUIRES
    "libGLX.so.0()(64bit), libOpenGL.so.0()(64bit), libQt6Core.so.6()(64bit), libQt6Core.so.6(Qt_6)(64bit), libQt6Core.so.6(Qt_6.3)(64bit), libQt6Gui.so.6()(64bit), libQt6Gui.so.6(Qt_6)(64bit), libQt6Widgets.so.6()(64bit), libQt6Widgets.so.6(Qt_6)(64bit), libc.so.6()(64bit), libc.so.6(GLIBC_2.2.5)(64bit), libc.so.6(GLIBC_2.34)(64bit), libgcc_s.so.1()(64bit), libgcc_s.so.1(GCC_3.0)(64bit), libm.so.6()(64bit), libstdc++.so.6()(64bit), libstdc++.so.6(CXXABI_1.3)(64bit), libstdc++.so.6(CXXABI_1.3.9)(64bit), libstdc++.so.6(GLIBCXX_3.4)(64bit), rpmlib(CompressedFileNames) <= 3.0.4-1, rpmlib(FileDigests) <= 4.6.0-1, rpmlib(PayloadFilesHavePrefix) <= 4.0-1, rpmlib(PayloadIsZstd) <= 5.4.18-1, rtld(GNU_HASH), "
)
set(CPACK_RPM_PACKAGE_AUTOREQPROV YES)
set(CPACK_RPM_PACKAGE_LICENSE MIT)
set(CPACK_RPM_PACKAGE_RELEASE 2)
set(CPACK_RPM_PACKAGE_PROVIDES "QssCreator")
set(CPACK_RPM_PACKAGE_GROUP "Development/Application")

include(CPack REQUIRED)
cpack_add_component("${PROJECT_NAME}"
    DISPALY_NAME "Qss Creator Installer"
    DESCRIPTION "Install Qss Creator"
    REQUIRED
)

if (WIN32)
    if (NOT EXISTS ${CMAKE_SOURCE_DIR}/tests/googletest/.git)
        find_package(Git)
        if (GIT_FOUND)
            execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init
                --recursive
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/googletest/
            RESULT_VARIABLE GOOGLE_TEST_OK)
        if(NOT GOOGLE_TEST_OK EQUAL "0")
            message(FATAL_ERROR "Could not update submodule googletest, aborting")
        endif()
        endif()
    endif()

    # Building googletest as object library
    set(gtest_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest)
    set(gmock_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googlemock)
    add_library(googletest OBJECT
        ${gtest_SOURCE_DIR}/src/gtest-all.cc
        ${gmock_SOURCE_DIR}/src/gmock-all.cc
    )
    target_include_directories(googletest PRIVATE
        ${gtest_SOURCE_DIR}
        ${gtest_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}
        ${gmock_SOURCE_DIR}/include
    )
endif()

enable_testing()

function(add_new_test testname)
    if (${ARGC} GREATER 1)
        add_executable(${testname}
            ${ARGN}
        )
        add_test(NAME ${testname} COMMAND ${testname})
        target_link_libraries(${testname} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
            Qt${QT_VERSION_MAJOR}::Test)
        target_include_directories(${testname} PRIVATE ../src)
    else()
        message(FATAL_ERROR "add_test_executable needs a test name and at "
            "least one source")
    endif()
endfunction()

function(add_new_gtest testname)
    if (WIN32)
        add_new_test(${testname}
            ${ARGN}
            $<TARGET_OBJECTS:googletest>
        )
    elseif (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
        add_new_test(${testname}
            ${ARGN}
        )
        target_link_libraries(${testname} PRIVATE gtest gmock)
    endif()
    target_include_directories(${testname} PRIVATE
        ${gtest_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
    )
endfunction()

add_new_gtest(TestQssdEditor
    tst_qssdeditor.cpp
    ../src/qssdeditor.cpp
    ../src/iqssdeditor.cpp
    ../src/iqssdprocessor.cpp
)

add_new_gtest(TestQssdFileOperations
    tst_qssdfileoperations.cpp
    tst_mockdocumentfile.h
    ../src/qssdfileoperations.cpp
    ../src/iqssdfileoperations.cpp
    ../src/idocumentfile.cpp
)

add_new_gtest(TestMainWindow
    tst_mainwindow.cpp
    tst_mockqssdeditor.h
    tst_mockqssdfileoperations.h
    tst_mockuserdialogs.h
    tst_mockqssdprocessor.h
    ../src/mainwindow.cpp
    ../src/widgetspreview.cpp
    ../src/idocumentfile.cpp
    ../src/iqssdeditor.cpp
    ../src/iqssdfileoperations.cpp
    ../src/iqssdprocessor.cpp
    ../src/iuserdialogs.cpp
    ../src/findreplacedialog.cpp
    ../src/qssdvariableitemdelegate.cpp
)

add_new_gtest(TestFindReplaceDialog
    tst_findreplacedialog.cpp
    ../src/findreplacedialog.cpp
)

add_new_gtest(TestQssdVariable
    tst_qssdvariable.cpp
    ../src/qssdvariable.cpp
)

add_new_gtest(TestQssdProcessor
    tst_qssdprocessor.cpp
    tst_mockqssdvariablesmodle.h
    ../src/iqssdvariablesmodel.cpp
    ../src/qssdprocessor.cpp
    ../src/iqssdprocessor.cpp
)

add_new_gtest(TestQssdVariablesModel
    tst_qssdvariablesmodel.cpp
    ../src/qssdvariablesmodel.cpp
    ../src/iqssdvariablesmodel.cpp
)

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
    add_new_test(${testname} ${ARGN})
    target_link_libraries(${testname} PRIVATE gmock gtest)
endfunction()

add_new_test(TestQssdEditor
    tst_qssdeditor.cpp
    ../src/qssdeditor.cpp
    ../src/iqssdeditor.cpp
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
    ../src/mainwindow.cpp
    ../src/widgetspreview.cpp
    ../src/idocumentfile.cpp
    ../src/iqssdeditor.cpp
    ../src/iqssdfileoperations.cpp
)

add_new_gtest(TestFindReplaceDialog
    tst_findreplacedialog.cpp
    ../src/findreplacedialog.cpp
)

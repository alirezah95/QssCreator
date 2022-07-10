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
)

add_new_gtest(TestFindReplaceDialog
    tst_findreplacedialog.cpp
    ../src/findreplacedialog.cpp
)

add_new_gtest(TestQssdVariable
    tst_qssdvariable.cpp
    ../src/qssdvariable.cpp
)

add_new_gtest(TestQssdPreprocessor
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

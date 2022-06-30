enable_testing()

function(add_new_test_executable testname)
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

add_new_test_executable(TestQssdEditor
    tst_qssdeditor.cpp
    ../src/qssdeditor.cpp
    ../src/iqssdeditor.cpp
)

add_new_test_executable(TestQssdFileOperations
    tst_qssdfileoperations.cpp
    tst_mockdocumentfile.h
    ../src/qssdfileoperations.cpp
    ../src/iqssdfileoperations.cpp
    ../src/idocumentfile.cpp
)
target_link_libraries(TestQssdFileOperations PRIVATE gmock gtest)

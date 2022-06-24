enable_testing()

function(add_new_test_executable testname)
    if (${ARGC} GREATER 1)
        add_executable(${testname}
            ${ARGN}
        )
        add_test(NAME ${testname} COMMAND ${testname})
        target_link_libraries(${testname} PRIVATE Qt${QT_VERSION_MAJOR}::Gui
            Qt${QT_VERSION_MAJOR}::Test)
        target_include_directories(${testname} PRIVATE ../src)
    else()
        message(FATAL_ERROR "add_test_executable needs a test name and at "
            "least one source")
    endif()
endfunction()

add_new_test_executable(TestStyleSheetEditor
    tst_stylesheeteditor.cpp
)

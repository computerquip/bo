option(BO_BUILD_TESTS "Build tests for the stuff" ON)

if(BO_BUILD_TESTS)
    add_subdirectory(tests)
endif()
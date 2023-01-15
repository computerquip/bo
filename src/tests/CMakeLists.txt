project(bo-tests)

find_package(Catch2 3 REQUIRED)
add_executable(${PROJECT_NAME} bo_test.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        bo::bo
        Catch2::Catch2WithMain)
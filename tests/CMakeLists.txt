include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    game_tests
    game_tests.cpp
)

add_test(NAME GameTest COMMAND game_tests)

target_link_libraries(
    game_tests
    game
    GTest::gtest_main
)

target_include_directories(game_tests PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(game_tests)
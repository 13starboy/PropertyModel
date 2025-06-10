enable_testing()

file(GLOB TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(pm ${TEST_SOURCES})

target_link_libraries(pm
    PRIVATE
      pmodel
      GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(pm)

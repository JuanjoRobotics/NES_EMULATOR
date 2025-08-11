# tests/CMakeLists.txt


# Build Catch2 amalgamated as a static library (only compiled once)
add_library(catch2_amalgamated STATIC catch_amalgamated.cpp)
target_include_directories(catch2_amalgamated PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Test executable links against core and Catch2 static lib
add_executable(run_tests test_cpu.cpp)
target_link_libraries(run_tests PRIVATE nes_core catch2_amalgamated)
target_include_directories(run_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/include)

# Include directories for tests
# (nes_core already includes PUBLIC include)

# Register with CTest
add_test(NAME run_tests COMMAND run_tests)
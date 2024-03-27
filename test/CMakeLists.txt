# Include GoogleTest's CMakeLists.txt from the submodule
add_subdirectory(googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gmock_SOURCE_DIR}/include)
include_directories(../src)

# List all test source files
set(TEST_FILES
        matrixFunctionality.cpp
)

# Create an executable target for each test file
foreach (TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} gtest_main gmock_main)
endforeach ()
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

CONFIGURE_FILE(googletest-download/CMakeLists.txt.in googletest-download/CMakeLists.txt)
EXECUTE_PROCESS(
    COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/googletest-download
)

IF(result)
  MESSAGE(FATAL_ERROR "CMake step for googletest failed: ${result}")
ENDIF()

EXECUTE_PROCESS(
    COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/googletest-download
)

IF(result)
  MESSAGE(FATAL_ERROR "Build step for googletest failed: ${result}")
ENDIF()

# Prevent overriding the parent project's compiler/linker settings on Windows
SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

ADD_SUBDIRECTORY(
    ${CMAKE_BINARY_DIR}/googletest-src
    ${CMAKE_BINARY_DIR}/googletest-build
)

ADD_EXECUTABLE(test-executable src/test-executable.cxx)
TARGET_LINK_LIBRARIES(test-executable gtest_main)
ADD_TEST(NAME test-executable COMMAND test-executable)

SET_PROPERTY(
    TARGET
        test-executable
    PROPERTY CXX_STANDARD 14
)

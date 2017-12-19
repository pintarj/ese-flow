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

ADD_EXECUTABLE(test-channel src/test-channel.cxx)
TARGET_LINK_LIBRARIES(test-channel gtest_main)
ADD_TEST(NAME test-channel COMMAND test-channel)

ADD_EXECUTABLE(test-channel-consumer src/test-channel-consumer.cxx)
TARGET_LINK_LIBRARIES(test-channel-consumer gtest_main)
ADD_TEST(NAME test-channel-consumer COMMAND test-channel-consumer)

ADD_EXECUTABLE(test-executor src/test-executor.cxx)
TARGET_LINK_LIBRARIES(test-executor gtest_main)
ADD_TEST(NAME test-executor COMMAND test-executor)

ADD_EXECUTABLE(test-thread src/test-thread.cxx)
TARGET_LINK_LIBRARIES(test-thread ese-flow gtest_main)
ADD_TEST(NAME test-thread COMMAND test-thread)

SET_PROPERTY(
    TARGET
        test-channel
        test-channel-consumer
        test-executor
        test-thread
    PROPERTY CXX_STANDARD 14
)

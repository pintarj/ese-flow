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

ADD_EXECUTABLE(test-consumer src/test-consumer.cxx)
TARGET_LINK_LIBRARIES(test-consumer ese-flow gtest_main)
ADD_TEST(NAME test-consumer COMMAND test-consumer)

ADD_EXECUTABLE(test-executor src/test-executor.cxx)
TARGET_LINK_LIBRARIES(test-executor gtest_main)
ADD_TEST(NAME test-executor COMMAND test-executor)

ADD_EXECUTABLE(test-filter src/test-filter.cxx)
TARGET_LINK_LIBRARIES(test-filter gtest_main)
ADD_TEST(NAME test-filter COMMAND test-filter)

ADD_EXECUTABLE(test-filter-receiver src/test-filter-receiver.cxx)
TARGET_LINK_LIBRARIES(test-filter-receiver gtest_main)
ADD_TEST(NAME test-filter-receiver COMMAND test-filter-receiver)

ADD_EXECUTABLE(test-filter-sender src/test-filter-sender.cxx)
TARGET_LINK_LIBRARIES(test-filter-sender gtest_main)
ADD_TEST(NAME test-filter-sender COMMAND test-filter-sender)

ADD_EXECUTABLE(test-receiver src/test-receiver.cxx)
TARGET_LINK_LIBRARIES(test-receiver ese-flow gtest_main)
ADD_TEST(NAME test-receiver COMMAND test-receiver)

ADD_EXECUTABLE(test-sender src/test-sender.cxx)
TARGET_LINK_LIBRARIES(test-sender ese-flow gtest_main)
ADD_TEST(NAME test-sender COMMAND test-sender)

ADD_EXECUTABLE(test-thread src/test-thread.cxx)
TARGET_LINK_LIBRARIES(test-thread ese-flow gtest_main)
ADD_TEST(NAME test-thread COMMAND test-thread)

SET_PROPERTY(
    TARGET
        test-channel
        test-consumer
        test-executor
        test-filter
        test-filter-receiver
        test-filter-sender
        test-receiver
        test-sender
        test-thread
    PROPERTY CXX_STANDARD 14
)

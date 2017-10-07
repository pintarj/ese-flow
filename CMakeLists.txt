CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(ese-flow CXX)

INCLUDE_DIRECTORIES(include)

ADD_LIBRARY(ese-flow SHARED
    src/executor.cxx
    src/thread.cxx
    src/version.cxx
)
SET_PROPERTY(TARGET ese-flow PROPERTY CXX_STANDARD 14)

ADD_SUBDIRECTORY(test)

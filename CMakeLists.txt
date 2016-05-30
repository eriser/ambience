cmake_minimum_required (VERSION 2.6)
project (ambience)

# set (GoogletestBaseDir "test/googletest")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

#########
# GTest #
#########
# ADD_SUBDIRECTORY (${GoogletestBaseDir})
# include_directories(${GoogletestBaseDir}/googletest ${GoogletestBaseDir}/googletest/include)

###############
# Application #
###############
include_directories (
    src/util
    src/wav
    )

add_executable (bin/ambience
    src/wav/Codec.cc
    src/frontend/Frontend.cc
    )

#########
# Tests #
#########
# add_executable (bin/TestMatrix 
#     test/TestMatrix.cpp 
#     src/Matrix.cpp
#     )
# target_link_libraries (bin/TestMatrix gtest gtest_main)
# 
# add_executable (bin/TestTGV 
#     test/TestTGV.cpp
#     src/TGV.cpp
#     src/Parser.cpp
#     src/Matrix.cpp
#     )
# target_link_libraries (bin/TestTGV gtest gtest_main)


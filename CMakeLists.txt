cmake_minimum_required (VERSION 2.6)
project (ambience)

# set (GoogletestBaseDir "test/googletest")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

#########
# GTest #
#########
# ADD_SUBDIRECTORY (${GoogletestBaseDir})
# include_directories(${GoogletestBaseDir}/googletest ${GoogletestBaseDir}/googletest/include)

###############
# Application #
###############

file( MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin" )

include_directories (
    src/util
    src/wav
    src/synth
    )

add_subdirectory( src/synth )
add_subdirectory( src/wav )

add_executable (bin/ambience
    src/frontend/Frontend.cc
    )

target_link_libraries( bin/ambience 
    synth 
    wav 
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


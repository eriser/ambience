cmake_minimum_required (VERSION 2.6)
project (ambience)

# set (GoogletestBaseDir "test/googletest")

IF( CMAKE_COMPILER_IS_GNUCXX  )
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -g")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
ENDIF()

IF( MSVC )
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /O2")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
ENDIF()

#########
# GTest #
#########
# ADD_SUBDIRECTORY (${GoogletestBaseDir})
# include_directories(${GoogletestBaseDir}/googletest ${GoogletestBaseDir}/googletest/include)

###############
# Application #
###############

# file( MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin" )

# set(CORE_SOURCE)

set( CORE_HEADERS
	 src/core/Evolution.h
	 src/core/Representation.h
	 src/core/Rules.h
	)
	
set( FRONTEND_SOURCES
	 src/frontend/Frontend.cc
	)

include_directories (
    src/util
    src/wav
    src/synth
    src/core
    )
	
add_subdirectory( src/synth )
add_subdirectory( src/wav )

add_executable ( ambience
    ${CORE_HEADERS}
	${FRONTEND_SOURCES}
    )

target_link_libraries( ambience 
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


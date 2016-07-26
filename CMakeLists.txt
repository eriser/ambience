cmake_minimum_required (VERSION 2.6)
project (ambience)

# set (GoogletestBaseDir "test/googletest")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -O3")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -O3")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /O2 /W4")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /W4")
endif()

#find_package(OpenMP)
#if (OPENMP_FOUND)
#	add_definitions( -DAMBIENCE_OPENMP_ENABLED )
#    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}" )
#    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}" )
#	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}" )
#endif()

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

set( FX_HEADERS
	 src/fx/Delay.h
	)

include_directories (
    src/util
    src/wav
	src/cppsynth
    src/core
	src/fx
    )
	
add_subdirectory( src/synth )
add_subdirectory( src/wav )
add_subdirectory( src/cppsynth )

add_executable ( ambience
    ${CORE_HEADERS}
	${FRONTEND_SOURCES}
	${FX_HEADERS}
    )

target_link_libraries( ambience 
    wav
	cppsynth
    )


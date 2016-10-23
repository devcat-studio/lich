cmake_minimum_required (VERSION 2.8)

project (lich-test)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

aux_source_directory(../lich/ SOURCES)
aux_source_directory(../test/ SOURCES)
add_executable (lich-test ${SOURCES})

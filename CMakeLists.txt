cmake_minimum_required(VERSION 2.8)

# Options. Turn on with cmake -Dmyvarname=ON.
option(test "Build all tests." OFF)

set(PROJECT_NAME tag-grammar-finder)
project(${PROJECT_NAME})
#ExternalProject_Add(

# Require c++11 and libc++ for more helpers like initializer_list.
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -stdlib=libc++")

# Libraries & Executables.
add_library(basic_helper_lib em-training-example/BasicHelper.cc)
add_library(nlp_helper_lib em-training-example/NLPHelper.cc)
add_library(notation_lib em-training-example/Notation.cc)
add_library(edge_lib em-training-example/Edge.cc)
add_executable(${PROJECT_NAME} TagGrammarFinder.cc)

target_link_libraries(${PROJECT_NAME} nlp_helper_lib basic_helper_lib notation_lib edge_lib)

# Testing.
if (test)
#   add_executable(notation_test NotationTest.cc)
#   target_link_libraries(notation_test notation_lib)
endif()
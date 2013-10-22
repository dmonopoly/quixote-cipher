# CMakeLists for Quixote Cipher.
cmake_minimum_required(VERSION 2.8)

# Options. Turn on with cmake -Dmyvarname=ON.
option(test "Build all tests." OFF)

set(PROJECT_NAME sound-decipher)
set(TAG_GRAMMAR_FINDER tag-grammar-finder)
set(CYPHER_READER cypher-reader)
project(${PROJECT_NAME})

# Require c++11 and libc++ for more helpers like initializer_list.
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -stdlib=libc++")

# Libraries -> Executables -> Linking.
add_library(basic_helper_lib em-training-example/BasicHelper.cc)
add_library(nlp_helper_lib em-training-example/NLPHelper.cc)
add_library(notation_lib em-training-example/Notation.cc)
add_library(edge_lib em-training-example/Edge.cc)
add_library(tag_grammar_finder_lib TagGrammarFinder.cc)
add_library(cypher_reader_lib CypherReader.cc)
add_library(graph_aid_lib em-training-example/GraphAid.cc)
add_library(trellis_aid_lib em-training-example/TrellisAid.cc)

# add_executable(${CYPHER_READER} CypherReader.cc)
# add_executable(${TAG_GRAMMAR_FINDER} TagGrammarFinder.cc)
add_executable(${PROJECT_NAME} Main.cc)

# target_link_libraries(${TAG_GRAMMAR_FINDER}
#                       nlp_helper_lib
#                       basic_helper_lib
#                       notation_lib)
target_link_libraries(${PROJECT_NAME}
                      nlp_helper_lib
                      basic_helper_lib
                      notation_lib
                      edge_lib
                      tag_grammar_finder_lib
                      graph_aid_lib
                      cypher_reader_lib
                      trellis_aid_lib)

# Testing.
if (test)
#   add_executable(notation_test NotationTest.cc)
#   target_link_libraries(notation_test notation_lib)
endif()

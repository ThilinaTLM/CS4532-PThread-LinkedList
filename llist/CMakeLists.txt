cmake_minimum_required(VERSION 3.0)

project(llist)
set(HEADERS linked-list.h node.h safe-linked-list.h abstract-linked-list.h safe-fast-linked-list.h)

add_library(llist llist.h ${HEADERS} )

set_target_properties(llist PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(llist PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



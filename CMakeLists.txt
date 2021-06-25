cmake_minimum_required(VERSION 3.12) # version can be different

project(my_cpp_project) #name of your project

add_subdirectory(googletest) # add googletest subdirectory

include_directories(googletest/include) # this is so we can #include <gtest/gtest.h>

add_compile_options(-Wall -pedantic)

add_executable(mytests mytests.cpp compiler.cpp lexer.cpp) # add this executable

target_link_libraries(mytests PRIVATE gtest) # link google test to this executable

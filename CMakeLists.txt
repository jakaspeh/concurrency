cmake_minimum_required(VERSION 2.8)

project("Concurrency")

message("   ")
message(" ccccccccccccccccccccccccccccccccccccccccccc  ")
message(" c                                         c  ")
message(" c                concurency               c  ")
message(" c                                         c  ")
message(" ccccccccccccccccccccccccccccccccccccccccccc  ")
message("   ")



######################################################################
# SETTING CXX FLAGS
######################################################################

set(CMAKE_CXX_FLAGS "-Wall -pthread -std=c++11 -fopenmp ${CMAKE_CXX_FLAGS}")

file(GLOB source-files RELATIVE ${CMAKE_SOURCE_DIR} *.cpp)

foreach(file ${source-files})

  message(" found file: " ${file})
  string( REPLACE ".cpp" ""  file-name ${file})
  add_executable(${file-name} ${file})
  
endforeach(file ${source-files})
message("")




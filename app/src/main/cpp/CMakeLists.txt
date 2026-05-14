cmake_minimum_required(VERSION 3.10.2)
add_library(worldmodel SHARED NativeLib.cpp)
find_library(log-lib log)
target_link_libraries(worldmodel ${log-lib})
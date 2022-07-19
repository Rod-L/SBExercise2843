cmake_minimum_required(VERSION 3.22)
project(SBExercise2843)

set(CMAKE_CXX_STANDARD 14)

add_executable(SBExercise2843 src/main.cpp src/Order.cpp include/Order.h src/Cook.cpp include/Cook.h include/CoutAccess.h src/Restaurant.cpp include/Restaurant.h src/CoutAccess.cpp)

target_include_directories(SBExercise2843 PUBLIC include)

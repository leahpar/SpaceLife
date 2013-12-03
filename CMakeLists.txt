cmake_minimum_required(VERSION 2.6)

SET(PRJ  /space)
SET(EXECUTABLE_OUTPUT_PATH ${PRJ}/bin)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# lib Eigen - Stack aligment
# http://eigen.tuxfamily.org/dox-devel/group__TopicWrongStackAlignment.html
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mincoming-stack-boundary=2")

SET(CMAKE_BUILD_TYPE Release)

project(SpaceLife)

include_directories(
  ${PRJ}/include
  /usr/include/SDL2
)

file(
  GLOB_RECURSE
  src_files
  ${PRJ}/src/*
)

add_executable(
  spacelife
  ${src_files}
)

link_directories(
  /usr/lib/x86_64-linux-gnu
)

target_link_libraries(
  spacelife
  SDL2main
  SDL2
  SDL2_image
  SDL2_ttf
)


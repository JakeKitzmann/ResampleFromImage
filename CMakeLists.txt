cmake_minimum_required(VERSION 3.16)
project(ResampleFromImage
  VERSION 0.1.0
  LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
add_executable(ResampleFromImage src/main.cxx)
target_link_libraries(ResampleFromImage PRIVATE ${ITK_LIBRARIES})

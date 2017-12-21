cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules )

project( ZebrafishTrackerCalibration )

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

find_package( OpenCV 2 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(ZebrafishTrackerCalibration
  ${PROJECT_SOURCE_DIR}/src/ZebrafishTrackerCalibration.cpp)

target_link_libraries( ZebrafishTrackerCalibration ${OpenCV_LIBS} )

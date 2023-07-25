cmake_minimum_required(VERSION 3.5.1)

project(Yolov4)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./out")
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)
include_directories(
	./inc/
	/home/fuhao/opencv/opencv-4.6.0/include/
	/home/fuhao/yolov4/darkent/include/
	/usr/local/lib/
)
add_executable(main
	./src/main.cpp
	./src/object_detect.cpp

)

target_link_libraries(main PUBLIC -std=c++11 ${OpenCV_LIBS} Threads::Threads)


install(TARGETS main DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

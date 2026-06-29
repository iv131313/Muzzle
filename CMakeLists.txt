# 1. Set the minimum CMake version required to build this project
cmake_minimum_required(VERSION 3.5)

# 2. Name your project and specify the programming language
project(Muzzle LANGUAGES CXX)

# 3. Specify the C++ standard to use (optional but recommended)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

# 4. Define the executable target name and its source files
add_executable(basic_camera_test src/basic_camera_test.cpp)

target_link_libraries(basic_camera_test PRIVATE ${OpenCV_LIBS})
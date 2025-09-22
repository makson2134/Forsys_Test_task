cmake_minimum_required(VERSION 3.18)
project(StudentInfo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(cppzmq REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(httplib REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(gui_client)

file(COPY data DESTINATION ${CMAKE_BINARY_DIR})

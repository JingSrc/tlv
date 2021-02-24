cmake_minimum_required(VERSION 3.14)

project(tlv LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(./ src)

add_executable(${PROJECT_NAME} ${src})

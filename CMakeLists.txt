cmake_minimum_required(VERSION 3.19)

set(APP_NAME coro_worker)
project(${APP_NAME})

set(CONAN_DISABLE_CHECK_COMPILER ON)
include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 20)

add_executable(${APP_NAME} main.cpp)
target_link_libraries(${APP_NAME} ${CONAN_LIBS_BOOST})
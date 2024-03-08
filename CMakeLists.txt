cmake_minimum_required(VERSION 3.11)

project(logi_obs_plugin)

# Specify C++11 compile
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions (-D_WEBSOCKETPP_CPP11_SYSTEM_ERROR_)
add_definitions (-D_ASIO_STANDALONE_)

add_subdirectory(obs_plugin)

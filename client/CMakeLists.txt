cmake_minimum_required(VERSION 3.26)
project(client C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(../custom_libraries/custom_c_logger custom_c_logger_build)
add_subdirectory(../custom_libraries/custom_c_string custom_c_string_build)
add_subdirectory(../custom_libraries/system_info system_info_build)
add_subdirectory(libraries/client_network)
add_subdirectory(libraries/utils)

add_executable(client main.c)

target_include_directories(client PRIVATE ../custom_libraries/custom_c_logger)
target_include_directories(client PRIVATE ../custom_libraries/custom_c_string)
target_include_directories(client PRIVATE ../custom_libraries/system_info)
target_include_directories(client PRIVATE libraries/client_network)
target_include_directories(client PRIVATE libraries/utils)

target_link_libraries(client PRIVATE custom_c_string custom_c_logger client_network utils)

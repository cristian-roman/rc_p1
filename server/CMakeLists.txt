cmake_minimum_required(VERSION 3.26)
project(server C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(../custom_libraries/custom_c_logger custom_c_logger_build)
add_subdirectory(../custom_libraries/custom_c_string custom_c_string_build)
add_subdirectory(../custom_libraries/system_info system_info_build)
add_subdirectory(libraries/server_network)
add_subdirectory(libraries/client_handler)
add_subdirectory(libraries/web_handler)
add_subdirectory(libraries/web_handler/file_hierarchy_creator)
add_subdirectory(libraries/web_handler/downloader)
add_subdirectory(libraries/web_handler/url_extractor)

add_executable(server main.c)

target_include_directories(server PRIVATE ../custom_libraries/custom_c_logger)
target_include_directories(server PRIVATE ../custom_libraries/custom_c_string)
target_include_directories(server PRIVATE ../custom_libraries/system_info)
target_include_directories(server PRIVATE libraries/server_network)
target_include_directories(server PRIVATE libraries/client_handler)
target_include_directories(server PRIVATE libraries/web_handler)
target_include_directories(server PRIVATE libraries/web_handler/file_hierarchy_creator)
target_include_directories(server PRIVATE libraries/web_handler/downloader)
target_include_directories(server PRIVATE libraries/web_handler/url_extractor)

target_link_libraries(server PRIVATE custom_c_string custom_c_logger server_network downloader file_hierarchy_creator url_extractor client_handler web_handler)
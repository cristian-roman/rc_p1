# Root CMakeLists.txt

cmake_minimum_required(VERSION 3.26)
project(MyDownloader)

# Add subdirectories for X, Y, Z, and SERVER
add_subdirectory(custom_libraries)
add_subdirectory(server)

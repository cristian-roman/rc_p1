cmake_minimum_required(VERSION 3.26)
project(server C)

set(CMAKE_C_STANDARD 11)

# Add the executable
add_executable(server main.c)

# Define the libraries and paths
set(libraries myLogger server_network)
set(paths ../custom-libraries/myLogger libraries/server_network)

# Loop through the libraries
list(LENGTH libraries num_libraries)
math(EXPR num_libraries "${num_libraries}-1")

foreach(index RANGE ${num_libraries})
    list(GET libraries ${index} library)
    list(GET paths ${index} path)

    # Compile the library source file
    add_library(${library} SHARED ${path}/${library}.c)

    # Link the library to the executable
    target_link_libraries(server PRIVATE ${library})

    # Set the library output directory
    set_target_properties(${library} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

    message(STATUS "Added library ${library}")
endforeach()

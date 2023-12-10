cmake_minimum_required(VERSION 3.26)
project(server C)

set(CMAKE_C_STANDARD 11)

# Add the executable
add_executable(server main.c)

# Define the libraries and paths
set(libraries myLogger server_network myString system_info web_handler)
set(paths ../custom-libraries/myLogger libraries/server_network ../custom-libraries/myString ../custom-libraries/system_info libraries/web_handler)

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

    # Check if the library is server_network and link myLogger
    if(${library} STREQUAL "server_network")
        target_link_libraries(${library} PRIVATE myLogger)
    endif()

    if(${library} STREQUAL "myLogger")
        target_link_libraries(${library} PRIVATE myString system_info)
    endif()

    if(${library} STREQUAL "web_handler")
        target_link_libraries(${library} PRIVATE myLogger myString curl)
    endif()

    message(STATUS "Added library ${library}")
endforeach()

cmake_minimum_required(VERSION 3.26)
project(client C)

set(CMAKE_C_STANDARD 11)

# Add the executable
add_executable(client main.c)

# Define the libraries and paths
set(libraries myLogger myString system_info)
set(paths ../custom-libraries/myLogger ../custom-libraries/myString ../custom-libraries/system_info)

# Loop through the libraries
list(LENGTH libraries num_libraries )
math(EXPR num_libraries "${num_libraries}-1")

foreach(index RANGE ${num_libraries})
    list(GET libraries ${index} library)
    list(GET paths ${index} path)

    # Compile the library source file
    add_library(${library} SHARED ${path}/${library}.c)

    # Link the library to the executable
    target_link_libraries(client PRIVATE ${library})

    # Set the library output directory
    set_target_properties(${library} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

    if(${library} STREQUAL "myLogger")
        target_link_libraries(${library} PRIVATE myString system_info)
    endif()

    message(STATUS "Added library ${library}")
endforeach()

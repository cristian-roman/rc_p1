add_library(file_hierarchy_creator file_hierarchy_creator.c file_hierarchy_creator.h)
target_include_directories(file_hierarchy_creator PRIVATE custom_c_string custom_c_logger url_handler)
target_link_libraries(file_hierarchy_creator PRIVATE custom_c_string custom_c_logger url_handler)

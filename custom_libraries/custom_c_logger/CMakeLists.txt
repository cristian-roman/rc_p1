add_library(custom_c_logger custom_c_logger.c custom_c_logger.h)
target_link_libraries(custom_c_logger PRIVATE system_info)

add_library(utils utils.c utils.h)
target_include_directories(utils PRIVATE custom_c_string custom_c_logger)

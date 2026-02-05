add_executable(SandpileModel main.cpp)

target_link_libraries(SandpileModel PUBLIC str_functions parsing_argv resize_matrix tsv_parser matrix_converter)

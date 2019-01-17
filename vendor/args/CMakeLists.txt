cmake_minimum_required( VERSION 3.13.1 )

add_library( args INTERFACE )

# target_sources( args
#     INTERFACE include/args.hpp
# )

# target_include_directories( args
#     INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include
# )

target_compile_options( args
    INTERFACE -isystem ${CMAKE_CURRENT_LIST_DIR}/include
)

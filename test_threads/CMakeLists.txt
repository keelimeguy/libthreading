cmake_minimum_required( VERSION 3.13.1 )

set( PROJ_NAME sum )
add_executable( ${PROJ_NAME} "" )

set( COMPILE_FLAGS -std=c++14 ${OPT} )

set( HEADER_FILES
    include/cli.h
    include/Timer.h
    include/par_sum.h
)

set( SRC_FILES
    src/main.cpp
    src/par_sum.cpp
)

target_include_directories( ${PROJ_NAME}
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources( ${PROJ_NAME}
    PUBLIC ${HEADER_FILES}
    PRIVATE ${SRC_FILES}
)

target_link_libraries( ${PROJ_NAME}
    PRIVATE
        args
        threading
)

target_compile_options( ${PROJ_NAME}
    PRIVATE "${COMPILE_FLAGS}"
)

cmake_minimum_required( VERSION 3.13.1 )

set( PROJ_NAME threading )
add_library( ${PROJ_NAME} STATIC )

set( COMPILE_FLAGS -std=c++14 ${OPT} )

set( HEADER_FILES
    include/Barrier.h
    include/Condition.h
    include/Core.h
    include/Mutex.h
    include/RWLock.h
    include/Thread.h
)

set( SRC_FILES
    src/Core.cpp
)

target_include_directories( ${PROJ_NAME}
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries( ${PROJ_NAME}
    PRIVATE
        pthread
)

target_sources( ${PROJ_NAME}
    PUBLIC ${HEADER_FILES}
    PRIVATE ${SRC_FILES}
)

target_compile_options( ${PROJ_NAME}
    PRIVATE "${COMPILE_FLAGS}"
)

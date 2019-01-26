cmake_minimum_required( VERSION 3.13.1 )

project( other LANGUAGES CXX C )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib ) # Output directory for static lib (.LIB) (.A)
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin ) # Output directory for shared lib (.DLL)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin ) # Output directory for executables (.EXE)

set( OPT -Ofast )

add_subdirectory( vendor EXCLUDE_FROM_ALL )
add_subdirectory( libthreading )
add_subdirectory( test_threads )
add_subdirectory( sample_philosophers )

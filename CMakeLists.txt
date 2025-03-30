cmake_minimum_required(VERSION 3.10)
project(axon VERSION 1.0.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Find source files
file(GLOB_RECURSE LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.c")
file(GLOB MAIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# Create static library
add_library(axon_lib STATIC ${LIB_SOURCES})

# Create executable
add_executable(axon ${MAIN_SOURCES})
target_link_libraries(axon axon_lib m)

# Install targets
install(TARGETS axon DESTINATION bin)

# Install man pages if they exist
install(FILES man/axon.1 DESTINATION share/man/man1 OPTIONAL)

# Display status message
message(STATUS "Axon: AES Encryption Tool")
message(STATUS "Configuration complete")
cmake_minimum_required(VERSION 3.10)
project(axon VERSION 1.0.0 LANGUAGES C)

# Set C standard (applies to all builds)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Windows cross-compilation only if WINDOWS_BUILD is defined
if(DEFINED WINDOWS_BUILD)
    message(STATUS "Configuring for Windows cross-compilation")
    set(CMAKE_SYSTEM_NAME Windows)
    
    # Find the compiler - check common locations
    find_program(MINGW_GCC x86_64-w64-mingw32-gcc)
    if(MINGW_GCC)
        set(CMAKE_C_COMPILER ${MINGW_GCC})
    else()
        message(FATAL_ERROR "MinGW GCC compiler not found. Please install mingw-w64.")
    endif()
    
    # Find the C++ compiler similarly if needed
    find_program(MINGW_GPP x86_64-w64-mingw32-g++)
    if(MINGW_GPP)
        set(CMAKE_CXX_COMPILER ${MINGW_GPP})
    endif()
    
    # Find the resource compiler
    find_program(MINGW_WINDRES x86_64-w64-mingw32-windres)
    if(MINGW_WINDRES)
        set(CMAKE_RC_COMPILER ${MINGW_WINDRES})
    endif()
    
    set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif()

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

# Windows executable needs .exe suffix
if(DEFINED WINDOWS_BUILD)
    set_target_properties(axon PROPERTIES SUFFIX ".exe")
endif()

# Install targets
install(TARGETS axon DESTINATION bin)

# Install man pages if they exist
install(FILES man/axon.1 DESTINATION share/man/man1 OPTIONAL)

# Display status message
message(STATUS "Axon: AES Encryption Tool")
message(STATUS "Configuration complete")

# Add packaging support for Windows
if(DEFINED WINDOWS_BUILD)
    include(InstallRequiredSystemLibraries)
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_DISPLAY_NAME "Axon Encryption Tool")
    set(CPACK_NSIS_PACKAGE_NAME "Axon")
    set(CPACK_GENERATOR "NSIS")
    include(CPack)
endif()
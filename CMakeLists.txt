cmake_minimum_required(VERSION 3.10)
project(AES C)

# Set C Standard
set(CMAKE_C_STANDARD 11)

# Include directories
include_directories(include)

add_executable(crypt
    src/main.c
    lib/utils/fileio.c
    lib/utils/memory.c
    lib/crypto/password.c
    lib/crypto/encryptor.c
    lib/crypto/decryptor.c
    lib/crypto/key_expansion.c
    lib/utils/conversion.c
    lib/crypto/confusion.c
    lib/crypto/diffusion.c
)

install(TARGETS crypt DESTINATION bin)
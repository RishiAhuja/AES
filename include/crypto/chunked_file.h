#ifndef CRYPTO_CHUNKED_FILE_H
#define CRYPTO_CHUNKED_FILE_H

#include <stdio.h>

typedef struct {
    char*** state;
    size_t num_state;
} ChunkedFile;

#endif
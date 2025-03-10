#ifndef UTILS_FILEIO_H
#define UTILS_FILEIO_H

#include <stdio.h>
#include "../../include/crypto/chunked_file.h"

FILE* open_file(const char* filename, const char* mode);
void flush_stream(FILE *file);
char* read_file(const char* filename);
void copy_file(FILE* source, FILE* destination);
void close_files(FILE *file[], int size);
void init_state(const char* filename, char** state);
void write_file(const char* filename, const char* content, const int content_size);
int chunk_writer(const char* filename, const char** chunks, size_t chunks_len);
void init_state_from_contents(const char* contents, char** state);
ChunkedFile file_chunker(const char* filename);

#endif // UTILS_FILEIO_H
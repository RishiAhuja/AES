#include "../../include/utils/fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include "../include/common/config.h"
#include "../include/common/failures.h"
#include "../include/utils/memory.h"
#include "../include/crypto/chunked_file.h"
#include <limits.h>
#include <string.h>


FILE* open_file(const char* filename, const char* mode){
    FILE *file = fopen(filename, mode); // Automatically allocates memory in heap instead of stack, so it won't get invalidated. 
    // The pointer variable file itself is on the stack, but what it points to (the FILE structure) is on the heap.
    // When the open_file() function returns, only the local variable file is destroyed, but the actual FILE structure in heap memory remains intact
    if(file == NULL) fprintf(stderr, "Error opening file: %s\n", filename);
    return file;
}

char* read_file(const char* filename){
    FILE *file = open_file(filename, "r");
    if(file == NULL) return NULL;

    size_t buffer_size = 1024;
    size_t used_size = 0;

    char* buffer = (char*)malloc(buffer_size);
    if(buffer == NULL){
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        fclose(file);
        return NULL;
    }

    int c = 0;
    while((c=fgetc(file)) != EOF){
        if(used_size + 1 >= buffer_size){
            buffer_size *= 2;
            char* new_buffer = (char*)realloc(buffer, buffer_size);
            if(new_buffer == NULL){
                fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
                free(buffer);
                fclose(file);
                return NULL;
            }
            buffer = new_buffer;
        }
        buffer[used_size++] = (char)c;
    }
    buffer[used_size++] = '\0';
    fclose(file);
    return buffer;
}

void flush_stream(FILE *file){
    int c;
    // The file position is automatically advanced after each fgetc call. This happens internally within the fgetc function
    // The next call to fgetc will read from the new position
    while((c = fgetc(file)) != EOF){
        // printf("Character: %d, Position: %ld\n", c, ftell(filea));
        putchar(c);
    }
    rewind(file);
}

void copy_file(FILE* source, FILE* destination) {
    int c;
    while ((c = fgetc(source)) != EOF) {
        fputc(c, destination);
    }
    rewind(source); // Rewind the position of the file pointer to the beginning of the file
}

void close_files(FILE *file[], int size){
    for (size_t i = 0; i < size; i++)
    {
        if(fclose(file[i]) != 0){
            fprintf(stderr, "Error closing file\n");
        }
    }
}

void write_file(const char* filename, const char* content, const int content_size){
    FILE *file = open_file(filename, "w");
    if(file == NULL) return;
    for (size_t i = 0; i < content_size; i++){
        fputc(content[i], file);
    }
    fclose(file);
}

void append_file(const char* filename, const char* content, const int content_size){
    FILE *file = open_file(filename, "a");
    if(file == NULL) return;
    for (size_t i = 0; i < content_size; i++){
        fputc(content[i], file);
    }
    fclose(file);
}

void init_state(const char* filename, char** state){
    FILE *file = open_file(filename, "r");
    if(file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return;
    };
    int c;
    for (size_t i = 0; i < STATE_SIZE; i++){
        for (size_t j = 0; j < STATE_SIZE; j++){
            c = fgetc(file);
            if(c == EOF){
                state[i][j] = 0;
                return;
            }
            else{
                state[i][j] = (char)c;
            }
        }  
    } 
    fclose(file);
}

void init_state_from_contents(const char* contents, char** state){
    for (size_t i = 0; i < STATE_SIZE; i++)
    {
        for (size_t j = 0; j < STATE_SIZE; j++){
            state[i][j] = contents[i * STATE_SIZE + j];
        }
    }
}


ChunkedFile file_chunker(const char* filename){
    ChunkedFile result = {NULL, 0};
    char* file_contents = read_file(filename);
    int buffer = DEFAULT_BUFFER;

    size_t file_length = strlen(file_contents);
    size_t block_size = STATE_SIZE * STATE_SIZE;
    size_t num_blocks = (file_length + block_size - 1) / block_size;

    char* current_position = file_contents;

    size_t i = 0;
    char*** states = malloc(buffer * sizeof(char**));
    if (!states) {
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        free(file_contents);
        return result;
    }

    size_t remaining_length = strlen(file_contents);
    
    while(remaining_length > 0){
        if (i > buffer) {
            if (buffer > INT_MAX / 2) {
                fprintf(stderr, "Buffer size too large to double\n");
                return result;
            }
            int new_buffer = buffer * 2;
            printf("expanding buffer from %d to %d\n", buffer, new_buffer);
            char*** new_states = realloc(states, new_buffer * sizeof(char**));
            if (!new_states) {
                fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
                for (size_t j = 0; j < i; j++) {
                    free_matrix_memory(states[j], STATE_SIZE);
                }
                free(states);
                free(file_contents);
                return result;
            }
            buffer = new_buffer;
            states = new_states;
        }
        char** state = allocate_matrix_memory(STATE_SIZE, STATE_SIZE);
        if (!state) {
            fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
            for (size_t j = 0; j < i; j++) {
                free_matrix_memory(states[j], STATE_SIZE);
            }
            free(file_contents);
            return result;
        }

        init_state_from_contents(current_position, state);
        states[i++] = state;
        current_position += block_size;
        if(remaining_length > block_size){
            remaining_length -= block_size;
        }else{
            remaining_length = 0;
        }
        if (*current_position == '\0') break;
    }
    result.state = states;
    result.num_state = i;
    return result;
}


int chunk_writer(const char* filename, const char** chunks, size_t chunks_len){
    if (!filename || !chunks) {
        fprintf(stderr, INVALID_CHUNK_WRITER_ARGS);
        return EXIT_FAILURE;
    }
    if (chunks[0]) {
        size_t chunk_length = strlen(chunks[0]);
        write_file(filename, chunks[0], chunk_length);
    }
   for (size_t i = 1; i < chunks_len; i++){
    if (!chunks[i]) {
        fprintf(stderr, "Null chunk encountered at index %zu\n", i);
        return EXIT_FAILURE;
     }
     size_t chunk_length = strlen(chunks[i]);
    append_file(filename, chunks[i], chunk_length);
   }
   return EXIT_SUCCESS;
}
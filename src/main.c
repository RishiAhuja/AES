#include "../include/utils/fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../include/common/config.h"
#include "../include/common/failures.h"
#include "../include/utils/memory.h"
#include "../include/crypto/password.h"
#include "../include/utils/conversion.h"
#include "../include/crypto/encryptor.h"
#include "../include/crypto/decryptor.h"
#include "../include/crypto/chunked_file.h"
#include "../include/crypto/key_expansion.h"
#include "../include/crypto/confusion.h"

#define STATE_SIZE 4

// stderr is a standard error stream in C, 
// which is used to output error messages. 
// Unlike stdout, which is buffered, stderr 
// is typically unbuffered, meaning that error
// messages are displayed immediately without 
// waiting for the buffer to fill up. 
// This ensures that error messages are printed 
// promptly, even if the program crashes 
// or terminates unexpectedly



int main(int argc, const char* argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file> <key> <e/d>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *source_file = NULL;
    FILE *destination_file = NULL;
    int status = EXIT_SUCCESS;
    
    // char **state = allocate_matrix_memory(STATE_SIZE, STATE_SIZE);
    // init_state(argv[1], state);

    // char* final_pass = validate_password(argv[3]);

    // printf("\nFinal password length: %zu\n", strlen(final_pass));

    // char **password = allocate_matrix_memory(STATE_SIZE, STATE_SIZE);

    // for (size_t i = 0; i < STATE_SIZE; i++){
    //     for (size_t j = 0; j < STATE_SIZE; j++){
    //        state[i][j] ^= final_pass[i * STATE_SIZE + j];
    //     }
    // }
    // char* flat_state = malloc(STATE_SIZE * STATE_SIZE * sizeof(char));
    // if (flat_state == NULL) {
    //     fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
    //     free_matrix_memory(password, STATE_SIZE);
    //     free_matrix_memory(state, STATE_SIZE);
    //     return EXIT_FAILURE;
    // }

    // for (size_t i = 0; i < STATE_SIZE; i++) {
    //     for (size_t j = 0; j < STATE_SIZE; j++) {
    //         flat_state[i * STATE_SIZE + j] = state[i][j];
    //     }
    // }

    // char* hexarg = bytes_to_hex((unsigned char*)flat_state, STATE_SIZE * STATE_SIZE);
    // printf("Hex: %s\n", hexarg);

    // write_file(argv[2], hexarg, STATE_SIZE * STATE_SIZE * 2);

    // free_matrix_memory(password, STATE_SIZE);
    // free_matrix_memory(state, STATE_SIZE);
    

    // if(strcmp(argv[4], "e") == 0){
    //     // perform encryption of argv[1] with argv[3] and write to argv[2]
    //     ChunkedFile chunked_file = file_chunker(argv[1]);
    //     if (!chunked_file.state || chunked_file.num_state == 0) {
    //         fprintf(stderr, FILE_PROCESSING_FAILURE);
    //         return EXIT_FAILURE;
    //     }
    //     char* final_pass = validate_password(argv[3]);
    //     if (!final_pass) {
    //         fprintf(stderr, PASSWORD_VAL_FAILURE);
    //         for (size_t i = 0; i < chunked_file.num_state; i++) {
    //             free_matrix_memory(chunked_file.state[i], STATE_SIZE);
    //         }
    //         free(chunked_file.state);
    //         return EXIT_FAILURE;
    //     }
    //     const char** encrypted_content = chain_encryptor(chunked_file.state, final_pass, STATE_SIZE, chunked_file.num_state);
    //     if (!encrypted_content) {
    //         fprintf(stderr, ENCRYPTION_FAILURE);
    //         for (size_t i = 0; i < chunked_file.num_state; i++) {
    //             free_matrix_memory(chunked_file.state[i], STATE_SIZE);
    //         }
    //         free(chunked_file.state);
    //         free(final_pass);
    //         return EXIT_FAILURE;
    //     }
    //     int write_result = chunk_writer(argv[2], encrypted_content, chunked_file.num_state);
    //     if (write_result != 0) {
    //         fprintf(stderr, FILE_WRITE_FAILURE);
    //     }
    //     for (size_t i = 0; i < chunked_file.num_state; i++) {
    //         free_matrix_memory(chunked_file.state[i], STATE_SIZE);
    //         free(encrypted_content[i]);
    //     }
    //     free(chunked_file.state);
    //     free(encrypted_content);
    //     free(final_pass);
    // }
    // else if(strcmp(argv[4], "d") == 0){
    //     char* encrypted_content = read_file(argv[1]);
    //     if (!encrypted_content) {
    //         fprintf(stderr, FILE_PROCESSING_FAILURE);
    //         return EXIT_FAILURE;
    //     }
        
    //     size_t num_chunks = 0;
    //     char** encrypted_chunks = parse_encrypted_file(encrypted_content, &num_chunks);
    //     if (!encrypted_chunks) {
    //         fprintf(stderr, FILE_PROCESSING_FAILURE);
    //         free(encrypted_content);
    //         return EXIT_FAILURE;
    //     }
        
    //     char* final_pass = validate_password(argv[3]);
    //     if (!final_pass) {
    //         fprintf(stderr, PASSWORD_VAL_FAILURE);
    //         for (size_t i = 0; i < num_chunks; i++) {
    //             free(encrypted_chunks[i]);
    //         }
    //         free(encrypted_chunks);
    //         free(encrypted_content);
    //         return EXIT_FAILURE;
    //     }
        
    //     char** decrypted_content = chain_decrytor(encrypted_chunks, final_pass, STATE_SIZE, num_chunks);
    //     if (!decrypted_content) {
    //         fprintf(stderr, "Decryption failed\n");
    //         for (size_t i = 0; i < num_chunks; i++) {
    //             free(encrypted_chunks[i]); 
    //         }
    //         free(encrypted_chunks);
    //         free(encrypted_content);
    //         free(final_pass);
    //         return EXIT_FAILURE;
    //     }
        
    //     int write_result = chunk_writer(argv[2], (const char**)decrypted_content, num_chunks);
    //     if (write_result == -1) {
    //         fprintf(stderr, FILE_WRITE_FAILURE);
    //     }
        
    //     for (size_t i = 0; i < num_chunks; i++) {
    //         free(encrypted_chunks[i]); 
    //         free(decrypted_content[i]);
    //     }
    //     free(encrypted_chunks);
    //     free(decrypted_content);
    //     free(encrypted_content);
    //     free(final_pass);
    // }
    // else{
    //     printf("%s\n\n\n", argv[4]);
    //     fprintf(stderr, "Invalid operation\n");
    //     status = EXIT_FAILURE;
    // }

    char* final_pass = validate_password(argv[3]);

    char* expanded_key = malloc(176);
    if (expanded_key == NULL) {
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        free(final_pass);
        return EXIT_FAILURE;
    }

    expand_key(final_pass, 16, expanded_key, EXPANDED_KEY_SIZE);
    if (expanded_key == NULL) {
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        free(final_pass);
        return EXIT_FAILURE;
    }
    
    print_expanded_key((const uint8_t *)expanded_key);

    char* state = read_file(argv[1]);
    char** state_t = allocate_matrix_memory(STATE_SIZE, STATE_SIZE);

    for (size_t i = 0; i < STATE_SIZE; i++)
    {
        for (size_t j = 0; j < STATE_SIZE; j++)
        {
            state_t[i][j] = state[i*STATE_SIZE + j];
        }
        
    }
    
    apply_rounds_keys(state_t, expanded_key);
    print_confused_state(state_t);

    return status;
}
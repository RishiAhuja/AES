#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/common/failures.h"
#include "../../include/crypto/decryptor.h"
#include "../../include/crypto/decryptor.h"
#include "../../include/utils/conversion.h"
#include "../../include/utils/memory.h"
#include "../../include/common/config.h"

char* chunk_decryptor(char* hex_bytes, char* final_pass, int block_size){
    size_t binary_len;
    unsigned char* binary_data = hex_to_bytes(hex_bytes, &binary_len);
    if (binary_data == NULL) {
        fprintf(stderr, "Error converting hex to bytes\n");
        return NULL;
    }
    char** state = allocate_matrix_memory(block_size, block_size);
    if (state == NULL) {
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        free(binary_data);
        return NULL;
    }
    for (size_t i = 0; i < block_size; i++) {
        for (size_t j = 0; j < block_size; j++) {
            state[i][j] = binary_data[i * block_size + j];
        }
    }
    single_state_decryption(state, final_pass);
    free(binary_data);

    char* flat_state = (char*)malloc(block_size * block_size * sizeof(char));
    if (flat_state == NULL) {
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        free_matrix_memory(state, block_size);
        return NULL;
    }
    for (size_t i = 0; i < block_size; i++) {
        for (size_t j = 0; j < block_size; j++) {
            flat_state[i * block_size + j] = state[i][j];
        }
    }
    free_matrix_memory(state, block_size);
    return flat_state;
}


char** chain_decryptor(char** hex_file_data, char* initial_pass, int block_size, int num_states){
    char** decrypted_flat_content = (char**)malloc(num_states * sizeof(char*));
    if (decrypted_flat_content == NULL) {
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        return NULL;
    }
    
    char* current_pass = malloc(strlen(initial_pass) + 1);
    if (current_pass == NULL) {
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        free(decrypted_flat_content);
        return NULL;
    }
    strcpy(current_pass, initial_pass);
    
    for (size_t i = 0; i < num_states; i++){
        decrypted_flat_content[i] = chunk_decryptor(hex_file_data[i], current_pass, block_size);
        
        if (decrypted_flat_content[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(decrypted_flat_content[j]);
            }
            free(decrypted_flat_content);
            free(current_pass);
            return NULL;
        }
        
        if (i < num_states - 1) {
            free(current_pass); 
            current_pass = malloc(strlen(hex_file_data[i]) + 1);
            if (current_pass == NULL) {
                fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
                for (size_t j = 0; j <= i; j++) {
                    free(decrypted_flat_content[j]);
                }
                free(decrypted_flat_content);
                return NULL;
            }
            strcpy(current_pass, hex_file_data[i]);
        }
    }
    return decrypted_flat_content;
}

char** parse_encrypted_file(const char* file_content, size_t* num_blocks_out){
    if (!file_content || !num_blocks_out) {
        fprintf(stderr, FILE_PARSE_FAILURE);
        return NULL;
    }
    size_t hex_block_size = STATE_SIZE * STATE_SIZE * 2;
    size_t file_length = strlen(file_content);
    size_t num_blocks = file_length / hex_block_size;
    if (file_length % hex_block_size != 0) {
        fprintf(stderr, "Warning: File length (%zu) is not a multiple of block size (%zu)\n", 
                file_length, hex_block_size);
    }
    char** blocks = malloc(num_blocks * sizeof(char*));
    if (!blocks) {
        return NULL;
    }
    for (size_t i = 0; i < num_blocks; i++)
    {
       blocks[i] = malloc(hex_block_size + 1);
        if (!blocks[i]) {
            for (size_t j = 0; j < i; j++) {
                free(blocks[j]);
            }
            free(blocks);
            return NULL;
        }
        memcpy(blocks[i], file_content + i * hex_block_size, hex_block_size);
        blocks[i][hex_block_size] = '\0';
    }
    *num_blocks_out = num_blocks;
    return blocks;
}

void single_state_decryption(char** state, char* final_key) {
    char* expanded_key = malloc(EXPANDED_KEY_SIZE);
    if (expanded_key == NULL) {
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        return;
    }
    expand_key(final_key, 16, expanded_key, EXPANDED_KEY_SIZE);
    add_round_key(state, expanded_key + (10 * STATE_SIZE * STATE_SIZE));
    for (size_t round = 0; round < 9; round++) {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(state, expanded_key + ((9 - round) * STATE_SIZE * STATE_SIZE));
        inv_mix_columns(state);
    }
    
    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_round_key(state, expanded_key);
    
    free(expanded_key);
}
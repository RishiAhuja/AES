#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/common/failures.h"
#include "../../include/crypto/encryptor.h"
#include "../../include/utils/conversion.h"


char* chunk_encryptor(char** state, char* final_pass, int block_size){
    for (size_t i = 0; i < block_size; i++){
        for (size_t j = 0; j < block_size; j++){
            state[i][j] ^= final_pass[i * block_size + j];
        }
    }

    char* flat_state = (char*)malloc(block_size * block_size * sizeof(char));
    if (flat_state == NULL) {
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        return NULL;
    }
    for (size_t i = 0; i < block_size; i++){
        for (size_t j = 0; j < block_size; j++){
            flat_state[i*block_size + j] = state[i][j];
        }
    }

    char* hexargs = bytes_to_hex((unsigned char*)flat_state, block_size * block_size);
    free(flat_state);
    return hexargs;
}

char** chain_encryptor(char*** states, char* initial_pass, int block_size, int num_states){
    char** encrypted_flat_hexstates = (char**)malloc(num_states * sizeof(char*));
    if (encrypted_flat_hexstates == NULL) {
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        return NULL;
    }
    char* current_pass = initial_pass; 
    memcpy(current_pass, initial_pass, strlen(initial_pass));
      
    for (size_t i = 0; i < num_states; i++){
        encrypted_flat_hexstates[i] = chunk_encryptor(states[i], current_pass, block_size);
        if (encrypted_flat_hexstates[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(encrypted_flat_hexstates[j]);
            }
            free(encrypted_flat_hexstates);
            return NULL;
        }
        current_pass = encrypted_flat_hexstates[i];
    }
    return encrypted_flat_hexstates;
}

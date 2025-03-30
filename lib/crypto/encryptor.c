#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/common/failures.h"
#include "../../include/common/config.h"
#include "../../include/crypto/encryptor.h"
#include "../../include/utils/conversion.h"
#include "../../include/crypto/confusion.h"
#include "../../include/crypto/diffusion.h"
#include "../../include/crypto/key_expansion.h"


char* chunk_encryptor(char** state, char* final_pass, int block_size){
    single_state_encyption(state, final_pass);

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
    char* current_pass = malloc(strlen(initial_pass) + 1);
    if (current_pass == NULL) {
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        free(encrypted_flat_hexstates);
        return NULL;
    }
    strcpy(current_pass, initial_pass);
      
    for (size_t i = 0; i < num_states; i++){
        encrypted_flat_hexstates[i] = chunk_encryptor(states[i], current_pass, block_size);
        if (encrypted_flat_hexstates[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(encrypted_flat_hexstates[j]);
            }
            free(encrypted_flat_hexstates);
            free(current_pass);
            return NULL;
        }
        current_pass = encrypted_flat_hexstates[i];
        if (current_pass == NULL) {
            fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
            for (size_t j = 0; j <= i; j++) {
                free(encrypted_flat_hexstates[j]);
            }
            free(encrypted_flat_hexstates);
            return NULL;
        }
    }
    return encrypted_flat_hexstates;
}


void single_state_encyption(char** state, char* final_key){
    char* expanded_key = malloc(EXPANDED_KEY_SIZE);
    if (expanded_key == NULL) {
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        return;
    }
    expand_key(final_key, 16, expanded_key, EXPANDED_KEY_SIZE);
    add_round_key(state, expanded_key);
    for (size_t round = 0; round < 10; round++) {
        sub_bytes(state);
        shift_rows(state);
        if (round != 9){
            mix_columns(state);
        }        
        add_round_key(state, expanded_key + ((round + 1) * STATE_SIZE * STATE_SIZE));
    }
    free(expanded_key);
}
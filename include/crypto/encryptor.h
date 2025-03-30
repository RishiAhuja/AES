#ifndef CRYPTO_ENCRYPTOR_H
#define CRYPTO_ENCRYPTOR_H

#include <stdio.h>

char* chunk_encryptor(char** state, char* final_pass, int block_size);
char** chain_encryptor(char*** states, char* initial_pass, int block_size, int num_states);
void single_state_encyption(char** state, char* final_key);

#endif // CRYPTO_ENCRYPTOR_H
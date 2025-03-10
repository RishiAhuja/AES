#ifndef CRYPTO_DECRYPTOR_H
#define CRYPTO_DECRYPTOR_H

#include <stdio.h>

char* chunk_decryptor(char* hex_bytes, char* final_pass, int block_size);
char** chain_decrytor(char** hex_file_data, char* initial_pass, int block_size, int num_states);
char** parse_encrypted_file(const char* file_content, size_t* num_blocks_out);

#endif // CRYPTO_ENCRYPTOR_H
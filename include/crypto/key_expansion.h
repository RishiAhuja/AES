#ifndef CRYPTO_KEY_EXPANSION_H
#define CRYPTO_KEY_EXPANSION_H

#include <stdio.h>
#include <stdint.h>

void expand_key(char* key, int key_size, char* expanded_key, int expanded_key_size);
void print_expanded_key(const uint8_t *expanded_key);

#endif // CRYPTO_KEY_EXPANSION_H
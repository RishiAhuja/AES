#ifndef CRYPTO_DIFFUSION_H
#define CRYPTO_DIFFUSION_H

#include <stdint.h>

#include "../../include/common/config.h"

void mix_columns(char** state);
void shift_rows(char** state);
void inv_mix_columns(char** state);

#endif // CRYPTO_DIFFUSION_H
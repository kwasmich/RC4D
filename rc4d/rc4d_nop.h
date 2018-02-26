#include <limits.h>



void rc4dEncrypt(uint8_t * const out_block, const uint8_t * const in_BLOCK, const uint8_t in_BLOCK_SIZE, const uint8_t * const in_KEY, const uint8_t in_KEY_SIZE) {
  memcpy(out_block, in_BLOCK, in_BLOCK_SIZE);
}



void rc4dDecrypt(uint8_t * const out_block, const uint8_t * const in_BLOCK, const uint8_t in_BLOCK_SIZE, const uint8_t * const in_KEY, const uint8_t in_KEY_SIZE) {
  memcpy(out_block, in_BLOCK, in_BLOCK_SIZE);
}


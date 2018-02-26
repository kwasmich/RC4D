#include <limits.h>



#define SWAP(a, b) do { uint8_t tmp = (a); (a) = (b); (b) = tmp; } while (0);



void rc4Transcrypt(uint8_t * const out_block, const uint8_t * const in_BLOCK, const uint8_t in_BLOCK_SIZE, const uint8_t * const in_KEY, const uint8_t in_KEY_SIZE) {
  uint8_t s[256];
  int i = 0;
  int j = 0;

  for (i = 0; i < 256; i++) {
    s[i] = i;
  }



  for (i = 0; i < 256; i++) {
    j = (j + s[i] + in_KEY[i % in_KEY_SIZE]) & 255;
    SWAP(s[i], s[j]);
  }

  i = 0;
  j = 0;
  uint8_t k = 0;
  uint8_t cnt = 0;

  while (cnt < in_BLOCK_SIZE) {
    i = (i + 1) & 255;
    j = (j + s[i]) & 255;
    SWAP(s[i], s[j]);
    k = s[(s[i] + s[j]) & 255];
    out_block[cnt] = in_BLOCK[cnt] ^ k;
    cnt++;
  }
}


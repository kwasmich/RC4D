#include <limits.h>



#define SWAP(a, b) do { uint8_t tmp = (a); (a) = (b); (b) = tmp; } while (0);
#define IV 42



static void transpose(uint8_t * const out_d, const uint8_t * const in_D, const int in_LEN) {
  for (int i = 0; i < in_LEN; i++) {
    out_d[i] = in_D[in_LEN - 1 - i];
  }
}








static void rc4cfbEncrypt(uint8_t * const out_block, const uint8_t * const in_BLOCK, const uint8_t in_BLOCK_SIZE, const uint8_t * const in_KEY, const uint8_t in_KEY_SIZE) {
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
  uint8_t cnt = 0;

  while (cnt < in_BLOCK_SIZE) {
    i = (i + 1) & 255;
    j = (j + s[i]) & 255;
    SWAP(s[i], s[j]);
    const uint8_t k = s[(s[i] + s[j]) & 255];

    if (cnt) {


      
      out_block[cnt] = in_BLOCK[cnt] ^ s[out_block[cnt - 1]] ^ k;
      
    } else {
      out_block[cnt] = in_BLOCK[cnt] ^ s[IV] ^ k;
    }

    cnt++;
  }
}



static void rc4cfbDecrypt(uint8_t * const out_block, const uint8_t * const in_BLOCK, const uint8_t in_BLOCK_SIZE, const uint8_t * const in_KEY, const uint8_t in_KEY_SIZE) {
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

    if (cnt) {
      out_block[cnt] = in_BLOCK[cnt] ^ s[in_BLOCK[cnt - 1]] ^ k;
    } else {
      out_block[cnt] = in_BLOCK[cnt] ^ s[IV] ^ k;
    }

    cnt++;
  }
}



inline void rc4dEncrypt(uint8_t * const out_block, const uint8_t * const in_BLOCK, const uint8_t in_BLOCK_SIZE, const uint8_t * const in_KEY, const uint8_t in_KEY_SIZE) {
  uint8_t intermediate[in_BLOCK_SIZE];
  rc4cfbEncrypt(out_block, in_BLOCK, in_BLOCK_SIZE, in_KEY, in_KEY_SIZE);
  transpose(intermediate, out_block, in_BLOCK_SIZE);
  rc4cfbEncrypt(out_block, intermediate, in_BLOCK_SIZE, in_KEY, in_KEY_SIZE);
}



inline void rc4dDecrypt(uint8_t * const out_block, const uint8_t * const in_BLOCK, const uint8_t in_BLOCK_SIZE, const uint8_t * const in_KEY, const uint8_t in_KEY_SIZE) {
  uint8_t intermediate[in_BLOCK_SIZE];
  rc4cfbDecrypt(out_block, in_BLOCK, in_BLOCK_SIZE, in_KEY, in_KEY_SIZE);
  transpose(intermediate, out_block, in_BLOCK_SIZE);
  rc4cfbDecrypt(out_block, intermediate, in_BLOCK_SIZE, in_KEY, in_KEY_SIZE);
}


#include <limits.h>



#define SWAP(a, b) do { uint8_t tmp = (a); (a) = (b); (b) = tmp; } while (0);
#define IV 42



static void transposeInplace(uint8_t * const d, const int in_LEN) {
  uint8_t i = 0;
  uint8_t j = in_LEN - 1;

  do {
    SWAP(d[i], d[j])
    i++;
    j--;
  } while (i < j);
}



static void rc4cfbTranscrypt(uint8_t * const out_block, const uint8_t * const in_BLOCK, const uint8_t in_BLOCK_SIZE, const uint8_t * const in_KEY, const uint8_t in_KEY_SIZE, const bool in_DECRYPT) {
  uint8_t s[256] = { 0 };
  uint8_t i = UCHAR_MAX;
  uint8_t j = 0;

  do {
    s[i] = i;
  } while (i--);

  i = 0;

  do {
    j = (j + s[i] + in_KEY[i % in_KEY_SIZE]);
    SWAP(s[i], s[j]);
  } while (++i & UCHAR_MAX);

  i = 0;
  j = 0;
  uint8_t cnt = 0;

  do {
    i = (i + 1) & 255;
    j = (j + s[i]) & 255;
    SWAP(s[i], s[j]);
    const uint8_t k = s[(s[i] + s[j]) & 255];

    if (cnt) {
      if (in_DECRYPT) {
        out_block[cnt] = in_BLOCK[cnt] ^ s[in_BLOCK[cnt - 1]] ^ k;
      } else {
        out_block[cnt] = in_BLOCK[cnt] ^ s[out_block[cnt - 1]] ^ k;
      }
    } else {
      out_block[cnt] = in_BLOCK[cnt] ^ s[IV] ^ k;
    }

    cnt++;
  } while (cnt < in_BLOCK_SIZE);
}



void rc4dEncrypt(uint8_t * const out_block, const uint8_t * const in_BLOCK, const uint8_t in_BLOCK_SIZE, const uint8_t * const in_KEY, const uint8_t in_KEY_SIZE) {
  uint8_t intermediate[in_BLOCK_SIZE];
  rc4cfbTranscrypt(intermediate, in_BLOCK, in_BLOCK_SIZE, in_KEY, in_KEY_SIZE, false);
  transposeInplace(intermediate, in_BLOCK_SIZE);
  rc4cfbTranscrypt(out_block, intermediate, in_BLOCK_SIZE, in_KEY, in_KEY_SIZE, false);
}



void rc4dDecrypt(uint8_t * const out_block, const uint8_t * const in_BLOCK, const uint8_t in_BLOCK_SIZE, const uint8_t * const in_KEY, const uint8_t in_KEY_SIZE) {
  uint8_t intermediate[in_BLOCK_SIZE];
  rc4cfbTranscrypt(intermediate, in_BLOCK, in_BLOCK_SIZE, in_KEY, in_KEY_SIZE, true);
  transposeInplace(intermediate, in_BLOCK_SIZE);
  rc4cfbTranscrypt(out_block, intermediate, in_BLOCK_SIZE, in_KEY, in_KEY_SIZE, true);
}


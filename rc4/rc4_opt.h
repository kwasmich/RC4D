#include <limits.h>



#define SWAP(a, b) do { uint8_t tmp = (a); (a) = (b); (b) = tmp; } while (0);



void rc4Transcrypt(uint8_t * const out_block, const uint8_t * const in_BLOCK, const uint8_t in_BLOCK_SIZE, const uint8_t * const in_KEY, const uint8_t in_KEY_SIZE) {
  uint8_t s[256] = {};
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
  uint8_t k = 0;
  uint8_t cnt = 0;

  do {
    i++;
    j += s[i];
    SWAP(s[i], s[j]);
    k = s[(s[i] + s[j]) & UCHAR_MAX];
    out_block[cnt] = in_BLOCK[cnt] ^ k;
    cnt++;
  } while (cnt < in_BLOCK_SIZE);
}


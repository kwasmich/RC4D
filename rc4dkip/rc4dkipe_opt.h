#include <EEPROM.h>
#include <limits.h>



#define SWAP(a, b) do { uint8_t tmp = (a); (a) = (b); (b) = tmp; } while (0);
#define IV 42



typedef uint8_t SBox_t[256];



static void transposeInplace(uint8_t * const d, const int in_LEN) {
  uint8_t i = 0;
  uint8_t j = in_LEN - 1;

  do {
    SWAP(d[i], d[j]);
    i++;
    j--;
  } while (i < j);
}



static void rc4cfbTranscryptInplace(uint8_t * const block, const uint8_t in_BLOCK_SIZE, const bool in_DECRYPT) {
  SBox_t s;
  uint8_t i = 0;
  uint8_t j = 0;
  uint8_t prev = 0;
  uint8_t prevPrev = 0;
  uint8_t cnt = 0;

  EEPROM.get(0x0000, s);

  do {
    i = (i + 1) & 255;
    j = (j + s[i]) & 255;
    SWAP(s[i], s[j]);
    const uint8_t k = s[(s[i] + s[j]) & 255];

    if (in_DECRYPT) {
      prev = prevPrev;
      prevPrev = block[cnt];
    }

    block[cnt] = block[cnt] ^ k;

    if (cnt) {
      if (in_DECRYPT) {
        block[cnt] ^= s[prev];
      } else {
        block[cnt] ^= s[block[cnt - 1]];
      }
    } else {
      block[cnt] ^= s[IV];
    }

    cnt++;
  } while (cnt < in_BLOCK_SIZE);
}



void rc4dInit(const uint8_t in_KEY[16]) {
  SBox_t s;
  uint8_t i = UCHAR_MAX;
  uint8_t j = 0;

  do {
    s[i] = i;
  } while (i--);

  i = 0;

  do {
    j = (j + s[i] + in_KEY[i & 0x0f]);
    SWAP(s[i], s[j]);
  } while (++i & UCHAR_MAX);

  EEPROM.put(0x0000, s);
}



void rc4dEncrypt(uint8_t * const block, const uint8_t in_BLOCK_SIZE) {
  rc4cfbTranscryptInplace(block, in_BLOCK_SIZE, false);
  transposeInplace(block, in_BLOCK_SIZE);
  rc4cfbTranscryptInplace(block, in_BLOCK_SIZE, false);
}



void rc4dDecrypt(uint8_t * const block, const uint8_t in_BLOCK_SIZE) {
  rc4cfbTranscryptInplace(block, in_BLOCK_SIZE, true);
  transposeInplace(block, in_BLOCK_SIZE);
  rc4cfbTranscryptInplace(block, in_BLOCK_SIZE, true);
}


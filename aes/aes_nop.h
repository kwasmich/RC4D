void aesEncrypt128(uint8_t out_block[16], const uint8_t in_BLOCK[16], const uint8_t in_KEY[16]) {
  memcpy(out_block, in_BLOCK, 16);
}



void aesDecrypt128(uint8_t out_block[16], const uint8_t in_BLOCK[16], const uint8_t in_KEY[16]) {
  memcpy(out_block, in_BLOCK, 16);
}


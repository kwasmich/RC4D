//#include "aes_plain.h" // 11040 ms   5596 flash    860 ram
//#include "aes_opt.h"   // 11516 ms   5560 flash    298 ram
//#include "aes_nop.h"   //    30 ms   3624 flash    298 ram

#include <stdio.h>

// create a FILE structure to reference our UART output function

static FILE uartout = {0} ;

// create a output function
// This works because Serial.write, although of
// type virtual, already exists.
static int uart_putchar (char c, FILE *stream)
{
  Serial.write(c) ;
  return 0 ;
}


static void printHex(const uint8_t * const buf, const uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    printf("%02x ", buf[i]);
  }

  puts("");
}



void setup() {
  // Start the UART
  Serial.begin(9600) ;

  // fill in the UART file descriptor with pointer to writer.
  fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);

  // The uart is the standard output device STDOUT.
  stdout = &uartout;

  uint8_t in[16] = "Hello World!___";
  uint8_t out[16] = "Hello World!___";
  // make the key volatile to be not optimized away in the NOP version
  volatile const uint8_t key[16] = "Key____________";

  puts((char *)in);
  aesEncrypt128(out, in, key);
  printHex(out, 16);
  aesDecrypt128(in, out, key);
  puts((char *)in);


  uint8_t in3[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
  uint8_t out3[16] = { 0 };
  const uint8_t in3Len = 16;
  // make the key volatile to be not optimized away in the NOP version
  volatile const uint8_t key3[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

  // verify the AES 128 bit test vector
  aesEncrypt128(out3, in3, key3);
  printHex(out3, in3Len);


  uint8_t in2[32] = { 0 };
  uint8_t out2[32] = { 0 };
  const uint8_t in2Len = 32;
  // make the key volatile to be not optimized away in the NOP version
  volatile const uint8_t key2[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10 };

  unsigned long start = millis();

  for (uint16_t i = 0; i < 1024; i++) {
    aesEncrypt128(&out2[0], &in2[0], key2);
    aesEncrypt128(&out2[16], &in2[16], key2);
    aesDecrypt128(&in2[0], &out2[0], key2);
    aesDecrypt128(&in2[16], &out2[16], key2);
  }

  unsigned long end = millis();
  printf("%d ms\n", end - start);
}

void loop() {
  // put your main code here, to run repeatedly:

}

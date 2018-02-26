//#include "rc4_plain.h" // 8856 ms   3954 flash    262 ram
//#include "rc4_opt.h"   // 4340 ms   3850 flash    262 ram
//#include "rc4_nop.h"   //   31 ms   3560 flash    262 ram

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

  uint8_t in[] = "Hello World!";
  uint8_t out[] = "Hello World!";
  // make the key volatile to be not optimized away in the NOP version
  volatile const uint8_t key[16] = "Key____________";
  const uint8_t inLen = strlen((char *)in);
  const uint8_t keyLen = strlen((char *)key);

  puts((char *)in);
  rc4Transcrypt(out, in, inLen, key, keyLen);
  printHex(out, inLen);
  rc4Transcrypt(in, out, inLen, key, keyLen);
  puts((char *)in);


  uint8_t in2[32] = { 0 };
  uint8_t out2[32] = { 0 };
  const uint8_t in2Len = 32;
  // make the key volatile to be not optimized away in the NOP version
  volatile const uint8_t key2[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10 };
  volatile const uint8_t key2Len = 16;

  // verify the RC4 128 bit test vector
  rc4Transcrypt(out2, in2, in2Len, key2, key2Len);
  printHex(out2, in2Len);

  unsigned long start = millis();

  for (uint16_t i = 0; i < 1024; i++) {
    rc4Transcrypt(out2, in2, in2Len, key2, key2Len);
    rc4Transcrypt(in2, out2, in2Len, key2, key2Len);
  }

  unsigned long end = millis();
  printf("%d ms\n", end - start);
}

void loop() {
  // put your main code here, to run repeatedly:

}

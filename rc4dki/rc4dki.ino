//#include "rc4dki_plain.h" // 3617 ms   3934 flash    262 ram
//#include "rc4dki_opt.h"   // 3482 ms   3900 flash    262 ram
//#include "rc4dki_nop.h"   //    0 ms   3408 flash    262 ram

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
  // make the key volatile to be not optimized away in the NOP version
  volatile const uint8_t key[16] = "Key____________";
  const uint8_t inLen = strlen((char *)in);

  puts((char *)in);
  rc4dEncrypt(in, inLen, key);
  printHex(in, inLen);
  rc4dDecrypt(in, inLen, key);
  puts((char *)in);


  uint8_t in2[32] = { 0 };
  const uint8_t in2Len = 32;
  // make the key volatile to be not optimized away in the NOP version
  volatile const uint8_t key2[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10 };

  // currently there is no 128 bit test vector to verify against
  rc4dEncrypt(in2, in2Len, key2);
  printHex(in2, in2Len);

  unsigned long start = millis();

  for (uint16_t i = 0; i < 1024; i++) {
    rc4dEncrypt(in2, in2Len, key2);
    rc4dDecrypt(in2, in2Len, key2);
  }

  unsigned long end = millis();
  printf("%d ms\n", end - start);
}

void loop() {
  // put your main code here, to run repeatedly:

}

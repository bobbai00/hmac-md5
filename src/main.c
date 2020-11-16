#include "hmac.h"

int main()
{
  unsigned char test[] = "text";
  unsigned char key[17];
  for (int i = 0; i < 16; i++)
  {
    key[i] = 0x0b;
  }
  unsigned char *output;
  int size;
  HMAC(key, 64, test, output, &size);
  print(output);
}
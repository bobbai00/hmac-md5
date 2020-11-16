#include "test.h"

void MD5_testcase()
{
  char *messages[] = {"", "a", "abc", "message digest", "abcdefghijklmnopqrstuvwxyz", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "12345678901234567890123456789012345678901234567890123456789012345678901234567890"};
  char *expectation[] = {"d41d8cd98f00b204e9800998ecf8427e", "0cc175b9c0f1b6a831c399e269772661", "900150983cd24fb0d6963f7d28e17f72", "f96b697d7cb7938d525a2f31aaf161d0", "c3fcd3d76192e4007dfb496cca67e13b", "d174ab98d277d9f5a5611c2c9f419d9f", "57edf4a22be3c955ac49da2e2107b67a"};
  for (int i = 0; i < 7; i++)
  {
    printf("---------- MD5 TESTCASE %d ---------\n", i + 1);
    printf("Input message: %s\n", messages[i]);
    printf("Expecting digest: %s\n", expectation[i]);
    printf("   Actual digest: ");
    print(md5(messages[i]));
    printf("-------------------------------------\n\n");
  }
};

void HMAC_testcase()
{
  unsigned char m1[] = "Hi There";
  unsigned char m2[] = "what do ya want for nothing?";
  unsigned char *msgs[3];
  unsigned char *keys[3];
  unsigned char key1[17], key2[17];
  unsigned char msg3[51];
  for (int i = 0; i < 50; i++)
  {
    msg3[i] = 0xDD;
  }
  msg3[50] = '\0';
  msgs[0] = m1;
  msgs[1] = m2;
  msgs[2] = msg3;
  for (int i = 0; i < 16; i++)
  {
    key1[i] = 0x0b;
    key2[i] = 0xAA;
  }
  key1[16] = '\0';
  key2[16] = '\0';
  keys[0] = key1;
  keys[1] = (unsigned char *)malloc(sizeof(unsigned char) * 5);
  keys[1][0] = 'J';
  keys[1][1] = 'e';
  keys[1][2] = 'f';
  keys[1][3] = 'e';
  keys[1][4] = '\0';
  keys[2] = key2;

  char *expectation[] = {"9294727a3638bb1c13f48ef8158bfc9d", "750c783e6ab0b503eaa86e310a5db738", "56be34521d144c88dbb8c733f0e8b3f6"};
  for (int i = 0; i < 3; i++)
  {
    printf("---------- HMAC TESTCASE %d ---------\n", i + 1);
    printf("Input message: %s\n", msgs[i]);
    printf("Key: %s\n", keys[i]);
    printf("Expecting digest: %s\n", expectation[i]);
    printf("   Actual digest: ");
    print(HMAC(keys[i], 64, msgs[i]));
    printf("-------------------------------------\n\n");
  }
};

int main()
{
  MD5_testcase();
  HMAC_testcase();
}
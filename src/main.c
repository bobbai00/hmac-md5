#include "HMD5.h"
#include "InputConverter.h"

int main()
{
  CV cv;
  for (int i = 0; i < 64; i++)
  {
    cv.msg[i] = i;
  }
  IV iv;
  iv.A = convertOctetToNumber(0x1, 0x23, 0x45, 0x67);
  iv.B = convertOctetToNumber(0x89, 0xab, 0xcd, 0xef);
  iv.C = convertOctetToNumber(0xfe, 0xdc, 0xba, 0x98);
  iv.D = convertOctetToNumber(0x76, 0x54, 0x32, 0x10);

  IV outcome = Hmd5(cv, iv);
}
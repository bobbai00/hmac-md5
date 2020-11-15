#include "HMD5.h"

num_t F(num_t b, num_t c, num_t d)
{

  return (b & c) | ((~b) & d);
};

num_t G(num_t b, num_t c, num_t d)
{
  return (b & d) | (c & (~d));
};

num_t H(num_t b, num_t c, num_t d)
{
  return (b ^ c ^ d);
};

num_t I(num_t b, num_t c, num_t d)
{
  return (c ^ (b | (~d)));
};

num_t rotate_left(const num_t value, int shift)
{
  if ((shift &= sizeof(value) * 8 - 1) == 0)
    return value;
  return (value << shift) | (value >> (sizeof(value) * 8 - shift));
}

IV Round(IV input, num_t idx, num_t xk[16], CV message, num_t s[16], num_t t[16])
{
  num_t (*g)(num_t, num_t, num_t);
  if (idx == 0)
  {
    g = &F;
  }
  else if (idx == 1)
  {
    g = &G;
  }
  else if (idx == 2)
  {
    g = &H;
  }
  else if (idx == 3)
  {
    g = &I;
  }

  for (int i = 0; i < 16; i++)
  {
    num_t g_out = g(input.B, input.C, input.D);
    num_t x = message.msg[xk[i]];
    num_t ti = t[i];
    num_t temp = rotate_left(g_out + x + input.A + ti, s[i]);
    num_t new_a = temp + input.B;

    // 循环右移
    input.A = input.D;
    input.B = new_a;
    input.C = input.B;
    input.D = input.C;
  }
  return input;
};

IV Hmd5(CV msg, IV iv)
{
  for (int i = 0; i < 4; i++)
  {
    iv = Round(iv, i, XK[i], msg, S[i], T[i]);
  }
  return iv;
};

IV GetIV0()
{
  IV iv;
  iv.A = convertOctetToNumber(0x1, 0x23, 0x45, 0x67);
  iv.B = convertOctetToNumber(0x89, 0xab, 0xcd, 0xef);
  iv.C = convertOctetToNumber(0xfe, 0xdc, 0xba, 0x98);
  iv.D = convertOctetToNumber(0x76, 0x54, 0x32, 0x10);

  return iv;
};

IV md5(char msg[])
{
  int group_idx = 0;
  long long group_num = 0;
  IV iv = GetIV0();
  while (1)
  {
    int base = group_idx * 64;
    int i = 0; // 当前分组的长度
    while (msg[i + base] != '\0' && i < 64)
    {
      i++;
    }

    if (i == 64)
    {
      // 说明不需要填充，当前分组的长度就为512bit
      char subMsg[64];
      memcpy(subMsg, &msg[base], 64);
      num_t *m = convertMsgToNumbers(subMsg, 64);
      CV cv;
      cv.msg = m;
      iv = Hmd5(cv, iv);
      group_idx++;
    }
    else
    {
      // 说明此时需要填充，填充 10000..., 长度使得该长度 % 512 = 448(%64 = 56)
      unsigned long long bit_number = i * 8 + base * 8;
      unsigned long long output = mod64(bit_number);
      num_t *klen = produce8bytes(output);

      if (i < 56)
      {

        char subMsg[64];
        memcpy(subMsg, &msg[base], i);
        num_t *m = convertMsgToNumbers(subMsg, i);
        // 填充1
        m[i] = 0x1000000;
        i++;
        // 填充0
        for (int j = 0; j < 56 - i - 1; j++)
        {
          m[i] = 0;
          i++;
        }
        // 填充64位的长度
        while (i < 64)
        {
          m[i] = klen[i - 56];
          i++;
        }

        // 填充完成
        CV cv;
        cv.msg = m;
        iv = Hmd5(cv, iv);
      }
      else
      {
        // 首先要把当前的分组填满，然后要创建一个新分组，填入56个字节的0和最终长度

        // 填满当前分组
        char subMsg[64];
        memcpy(subMsg, &msg[base], i);
        num_t *m = convertMsgToNumbers(subMsg, i);
        char subMsg1[64];
        memcpy(subMsg, &msg[base], i);
        num_t *m = convertMsgToNumbers(subMsg, i);
        // 填充1
        m[i] = 0x1000000;
        i++;
        // 填充0
        for (int j = 0; j < 64 - i - 1; j++)
        {
          m[i] = 0;
          i++;
        }
        CV cv;
        cv.msg = m;
        iv = Hmd5(cv, iv);

        // 创建新的分组
        num_t m2[64];
        for (int j = 0; j < 64; j++)
        {
          m2[j] = 0;
          if (j >= 56)
          {
            m2[j] = klen[j - 56];
          }
        }
        cv.msg = m2;
        iv = Hmd5(cv, iv);
      }
      break;
    }
  }
  return iv;
}

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

IV addIV(IV a, IV b)
{
  IV ret;
  ret.A = a.A + b.A;
  ret.B = a.B + b.B;
  ret.C = a.C + b.C;
  ret.D = a.D + b.D;
  return ret;
}

IV md5(char msg[])
{
  int group_idx = 0;
  IV iv = GetIV0();
  IV last_iv = GetIV0();
  while (1)
  {
    int base = group_idx * 64;
    int i = 0; // 当前分组的字节数
    while (msg[i + base] != '\0' && i < 64)
    {
      i++;
    }

    if (i == 64)
    {
      // 说明不需要填充，当前分组的长度就为512bit
      unsigned char subMsg[64];

      for (int i = 0; i < 64; i++)
      {
        subMsg[i] = 0;
      }
      memcpy(subMsg, &msg[base], 64);
      num_t *m = convertMsgToNumbers(subMsg);
      CV cv;
      cv.msg = m;
      iv = Hmd5(cv, iv);
      iv = addIV(iv, last_iv);
      last_iv = iv;
      group_idx++;
    }
    else
    {
      // 说明此时需要填充，填充 10000..., 长度使得该长度 % 512 = 448(%64 = 56)
      // 位数
      num_t bit_number = i * 8 + base * 8;
      num_t *klen = produceLastTwoWord(bit_number);

      // 说明这个分组长度小于448bit，需要首先填充1000到448 bit，再添加表示长度的字
      if (i < 56)
      {
        // 64字节
        unsigned char subMsg[64];
        for (int i = 0; i < 64; i++)
        {
          subMsg[i] = 0;
        }

        // 取出组
        memcpy(subMsg, &msg[base], i);

        // 首先填充一个10000000
        subMsg[i] = 0b10000000;

        i++;
        // 填充0
        int num_to_pad = 56 - i;
        for (int j = 0; j < num_to_pad; j++)
        {
          subMsg[i] = 0;
          i++;
        }

        // 转换成16个字
        num_t *m = convertMsgToNumbers(subMsg);
        m[14] = klen[0];
        m[15] = klen[1];

        // 填充完成
        CV cv;
        cv.msg = m;
        iv = Hmd5(cv, iv);
      }
      else
      {
        // 首先要把当前的分组填充到64个字节，然后要创建一个新分组，填入56个字节的0和最终长度
        // 填满当前分组
        unsigned char subMsg[64];
        for (int i = 0; i < 64; i++)
        {
          subMsg[i] = 0;
        }
        memcpy(subMsg, &msg[base], i);

        // 填充1
        subMsg[i] = 0b10000000;
        i++;
        // 填充0
        int num_to_pad = 64 - i;
        for (int j = 0; j < num_to_pad; j++)
        {
          subMsg[i] = 0;
          i++;
        }
        num_t *m = convertMsgToNumbers(subMsg);
        CV cv;
        cv.msg = m;
        iv = Hmd5(cv, iv);

        iv = addIV(iv, last_iv);
        last_iv = iv;
        // 创建新的分组
        num_t m2[16];
        for (int j = 0; j < 16; j++)
        {
          m2[j] = 0;
          if (j >= 14)
          {
            m2[j] = klen[j - 14];
          }
        }
        cv.msg = m2;
        iv = Hmd5(cv, iv);
      }
      break;
    }
  }
  IV ret = addIV(iv, last_iv);
  return ret;
}

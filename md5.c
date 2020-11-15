//
//  main.c
//  md5
//
//  Created by 白家栋 on 2020/11/15.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef unsigned int num_t;

typedef struct CV
{
  // 分组后的消息
  num_t *msg;

} CV;

num_t convertOctetToNumber(unsigned int o1, unsigned int o2, unsigned int o3, unsigned int o4)
{
  num_t res = o4 * 256 * 256 * 256 + o3 * 256 * 256 + o2 * 256 + o1;
  return res;
};

typedef struct IV
{
  num_t A;
  num_t B;
  num_t C;
  num_t D;
} IV;

IV Hmd5(CV msg, IV iv);

// 生成函数
num_t F(num_t b, num_t c, num_t d);

num_t G(num_t b, num_t c, num_t d);

num_t H(num_t b, num_t c, num_t d);

num_t I(num_t b, num_t c, num_t d);

// 循环左移
num_t rotate_left(const num_t value, int shift);

// input表示输入的IV，idx表示第几轮，
IV Round(IV input, num_t idx, num_t xk[16], CV message, num_t s[16], num_t t[16]);

num_t XK[4][16] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
    {1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12},
    {5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2},
    {0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9},
};

num_t T[4][16] = {{0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
                   0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8,
                   0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193,
                   0xa679438e, 0x49b40821},
                  {0xf61e2562, 0xc040b340, 0x265e5a51,
                   0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
                   0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905,
                   0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a},
                  {0xfffa3942, 0x8771f681,
                   0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60,
                   0xbebfbc70, 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
                   0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665},
                  {0xf4292244,
                   0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
                   0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314,
                   0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391}};

num_t S[4][16] = {
    {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7,
     12, 17, 22},
    {5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20},
    {4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23},
    {6, 10,
     15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21}};

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
    IV new_input;
    new_input.A = input.D;
    new_input.C = input.B;
    new_input.D = input.C;
    new_input.B = new_a;
    input = new_input;
  }
  return input;
};

IV Hmd5(CV msg, IV iv)
{
  for (int i = 0; i < 16; i++)
  {
    printf("%ud\n", msg.msg[i]);
  }
  for (int i = 0; i < 4; i++)
  {
    iv = Round(iv, i, XK[i], msg, S[i], T[i]);
  }
  return iv;
};

// 将一个word(4个字节)转换成一个4个字节的无符号整数

IV addIV(IV a, IV b)
{
  IV ret;
  ret.A = a.A + b.A;
  ret.B = a.B + b.B;
  ret.C = a.C + b.C;
  ret.D = a.D + b.D;
  return ret;
}

num_t *convertMsgToNumbers(unsigned char *msg)
{
  num_t *t = (num_t *)malloc(16 * sizeof(num_t));
  for (int i = 0; i < 16; i++)
  {
    num_t temp = convertOctetToNumber(msg[i * 4], msg[i * 4 + 1], msg[i * 4 + 2], msg[i * 4 + 3]);
    t[i] = temp;
  }
  return t;
};

unsigned long long mod64(unsigned long long len)
{
  unsigned long long i = pow(2, 64);
  return len % i;
}

num_t *produceLastTwoWord(unsigned long long length)
{
  num_t *ret = (num_t *)malloc(2 * sizeof(num_t));
  for (int i = 0; i < 2; i++)
  {
    ret[i] = 0;
  }
  unsigned int f = 0xffffffff;
  num_t first = f & length;
  ret[0] = first;
  length = length >> 32;
  ret[1] = length;

  return ret;
}

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

int main()
{
  md5("qwertyuiopasdfghjklzxcvbnmlkjhgfdsaqwertyuioplkjhgfdsazxadsjfhjlkadshjkfhjklasdhkflhjlkqhwioueryhiuoqewyuiotyuioadsjlkfljadsf");
}

#include "hmac.h"

unsigned char *ipad(int times)
{
  unsigned char *ret = (unsigned char *)malloc(sizeof(unsigned char) * times);
  for (int i = 0; i < times; i++)
  {
    ret[i] = 0x36;
  }
  return ret;
};

unsigned char *opad(int times)
{
  unsigned char *ret = (unsigned char *)malloc(sizeof(unsigned char) * times);
  for (int i = 0; i < times; i++)
  {
    ret[i] = 0x5c;
  }
  return ret;
};

unsigned char *padKey(unsigned char *key, int key_size, int B)
{
  unsigned char *ret = (unsigned char *)malloc(sizeof(unsigned char) * B);

  unsigned char *new_key = key;

  // 如果key的大小大于B, 需要先哈希 再padding
  if (key_size > B)
  {
    new_key = md5_hmac_version(key, key_size);
    key_size = 16;
  }
  for (int i = 0; i < B; i++)
  {
    ret[i] = 0;
    if (i < key_size)
    {
      ret[i] = key[i];
    }
  }
  return ret;
};

// 逐个异或
unsigned char *XOR(unsigned char *key, unsigned char *val, int size)
{
  unsigned char *ret = (unsigned char *)malloc(sizeof(unsigned char) * size);
  for (int i = 0; i < size; i++)
  {
    ret[i] = key[i] ^ val[i];
  }
  return ret;
}

// key为用户定义的密钥；B为长度，一般定为64；msg 为输入的消息；output为输出的数组
void HMAC(unsigned char *key, int B, unsigned char *msg, unsigned char *output, int *size)
{
  int key_size = 0;
  while (key[key_size] != '\0')
  {
    key_size++;
  }
  int msg_size = 0;
  while (msg[msg_size] != '\0')
  {
    msg_size++;
  }
  unsigned char *pad_key = padKey(key, key_size, B);
  unsigned char *h1 = (unsigned char *)malloc((msg_size + B) * sizeof(char));
  unsigned char *key1 = XOR(pad_key, ipad(B), B);

  // 拼接key和msg，大小为msg_size + B
  for (int i = 0; i < msg_size + B; i++)
  {
    h1[i] = 0;
    if (i < B)
    {
      h1[i] = key1[i];
    }
    else
    {
      h1[i] = msg[i - B];
    }
  }

  unsigned char *out1 = md5_hmac_version(h1, B + msg_size);
  unsigned char *h2 = (unsigned char *)malloc((16 + B) * sizeof(char));
  unsigned char *key2 = XOR(pad_key, opad(B), B);

  for (int i = 0; i < 16 + B; i++)
  {
    h2[i] = 0;
    if (i < B)
    {
      h2[i] = key1[i];
    }
    else
    {
      h2[i] = msg[i - B];
    }
  }

  output = md5_hmac_version(h2, B + 16);
  *size = B + 16;
};

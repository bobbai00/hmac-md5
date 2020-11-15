#ifndef INPUTCONVERTER_H
#define INPUTCONVERTER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define MAX_LENGTH 30000

typedef unsigned int num_t;

// 将一个word(4个字节)转换成一个4个字节的无符号整数
num_t convertOctetToNumber(int o1, int o2, int o3, int o4);

num_t *convertMsgToNumbers(char *msg, int length);

unsigned long long mod64(unsigned long long len)
{
  unsigned long long i = pow(2, 64);
  return len & i;
}

num_t *produce8bytes(unsigned long long length)
{
  num_t *ret = (num_t *)malloc(8 * sizeof(num_t));
  for (int i = 0; i < 8; i++)
  {
    ret[i] = 0;
  }
  int idx = 7;
  int modulos = pow(2, 8);
  while (length && idx > 0)
  {
    ret[idx] = length % modulos;
    idx--;
    length = length / modulos;
  }
  return ret;
}

typedef struct CV
{
  // 分组后的消息
  num_t *msg;

} CV;

#endif

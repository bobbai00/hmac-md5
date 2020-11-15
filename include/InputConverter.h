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

num_t *convertMsgToNumbers(unsigned char *msg);

num_t *produceLastTwoWord(unsigned long long length);

typedef struct CV
{
  // 分组后的消息
  num_t *msg;

} CV;

#endif

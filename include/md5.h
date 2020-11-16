#ifndef MD5_H
#define MD5_H
#define MAX_LENGTH 30000

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

typedef struct IV
{
  num_t A;
  num_t B;
  num_t C;
  num_t D;
} IV;

// 将一个word(4个字节)转换成一个4个字节的无符号整数
num_t convertOctetToNumber(unsigned int o1, unsigned int o2, unsigned int o3, unsigned int o4);

num_t *convertMsgToNumbers(unsigned char *msg);

num_t *produceLastTwoWord(unsigned long long length);

IV Hmd5(CV msg, IV iv);

IV GetIV0();

// 生成函数
num_t F(num_t b, num_t c, num_t d);

num_t G(num_t b, num_t c, num_t d);

num_t H(num_t b, num_t c, num_t d);

num_t I(num_t b, num_t c, num_t d);

// 循环左移
num_t rotate_left(const num_t value, int shift);

// input表示输入的IV，idx表示第几轮，
IV Round(IV input, num_t idx, num_t xk[16], CV message, num_t s[16], num_t t[16]);

unsigned char *decode(IV iv);

void print(unsigned char *res);

void copySubArray(unsigned char *dst, unsigned char *src, int start, int len);

unsigned char *md5(char *msg);

unsigned char *md5_hmac_version(unsigned char *input, long long size);

IV addIV(IV a, IV b);

void convertIVToString(IV src, unsigned char *dst);

#endif
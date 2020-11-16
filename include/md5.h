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
  // 分组后的消息, 共512bits
  num_t *msg;

} CV;

typedef struct IV
{
  num_t A;
  num_t B;
  num_t C;
  num_t D;
} IV;

// 将4个8位(有效位)的整数转换成一个4个字节的无符号整数
num_t convertOctetToNumber(unsigned int o1, unsigned int o2, unsigned int o3, unsigned int o4);

// 将消息转换成32位整数数组
num_t *convertMsgToNumbers(unsigned char *msg);

// 将消息长度转换成两个32位整数
num_t *produceLastTwoWord(unsigned long long length);

// 一个Hmd5压缩块
IV Hmd5(CV msg, IV iv);

// 获取初始向量
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

// 将向量转换为能够输出的char
unsigned char *decode(IV iv);

// 打印结果(16进制的结果，小端)
void print(unsigned char *res);

// 截取子数组
void copySubArray(unsigned char *dst, unsigned char *src, int start, int len);

// md5，用于将普通的字符串直接转成hash值
unsigned char *md5(char *msg);

// md5，用在hmac_md5中
unsigned char *md5_hmac_version(unsigned char *input, long long size);

// 向量相加
IV addIV(IV a, IV b);

#endif
#ifndef HMAC_H
#define HMAC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "md5.h"

// 获取ipad
unsigned char *ipad(int times);

// 获取opad
unsigned char *opad(int times);

// 获取padding后的key
unsigned char *padKey(unsigned char *key, int key_size, int B);

// HMAC主体
unsigned char *HMAC(unsigned char *key, int B, unsigned char *msg);

// 按位异或
unsigned char *XOR(unsigned char *key, unsigned char *val, int size);

#endif
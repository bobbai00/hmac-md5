#ifndef HMAC_H
#define HMAC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "md5.h"

unsigned char *ipad(int times);

unsigned char *opad(int times);

unsigned char *padKey(unsigned char *key, int key_size, int B);

void HMAC(unsigned char *key, int B, unsigned char *msg, unsigned char *output, int *size);

unsigned char *XOR(unsigned char *key, unsigned char *val, int size);

#endif
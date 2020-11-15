#include "InputConverter.h"

num_t convertOctetToNumber(int o1, int o2, int o3, int o4)
{
  num_t res = o1 * 256 * 256 * 256 + o2 * 256 * 256 + o3 * 256 + o4;
  return res;
};
num_t *convertMsgToNumbers(char *msg, int length)
{
  num_t *t = (num_t *)malloc(length * sizeof(num_t));
  for (int i = 0; i < length; i++)
  {
    t[i] = msg[i];
  }
  return t;
};

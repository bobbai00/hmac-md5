#include "InputConverter.h"

num_t convertOctetToNumber(unsigned int o1, unsigned int o2, unsigned int o3, unsigned int o4)
{
  num_t res = o4 * 256 * 256 * 256 + o3 * 256 * 256 + o2 * 256 + o1;
  return res;
};

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

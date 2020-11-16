#include "hmac.h"

int main()
{
  // usage of md5
  print(md5(""));
  print(md5("adfaiudsgufiouqeruioyquoiweyryo23491028kdjzbvnkabkdsbfajgfadsgfasdkhf"));

  // usage of HMAC
  unsigned char test[] = "F";
  unsigned char key[] = "3y:cr57T'F9F;NryW9ZPFBO}1aeM`\"B1opVx/v/00S_E_h2L8l]fMH+fA[Jy/-<";
  print(HMAC(key, 64, test));
}
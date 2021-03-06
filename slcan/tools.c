#include "tools.h"


static const long hextable[] = {
  [0 ... 255] = -1, // bit aligned access into this table is considerably
  ['0'] = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, // faster for most modern processors,
  ['A'] = 10, 11, 12, 13, 14, 15,       // for the space conscious, reduce to
  ['a'] = 10, 11, 12, 13, 14, 15        // signed char.
};

/**
 * @brief convert a hexidecimal string to a signed long
 * will not produce or process negative numbers except
 * to signal error.
 *
 * @param hex without decoration, case insensitive.
 *
 * @return -1 on error, or result (max (sizeof(long)*8)-1 bits)
 */
long hexdec(unsigned const char *hex, int chars)
{
  long ret = 0;
  int i = 0;
  while (i < chars && *hex && ret >= 0) {
    ret = (ret << 4) | hextable[*hex++];
    i++;
  }
  return ret;
}
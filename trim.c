#include "trim.h"

size_t trim(char *out, size_t len, const char *str)
{
  if(len == 0)
    return 0;
  const char *end;
  size_t out_size;
  while(isspace((unsigned char)*str)) str++;
  if(*str == 0) {
    *out = 0;
    return 1;
  }
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;
  end++;
  out_size = (end - str) < len-1 ? (end - str) : len-1;
  memcpy(out, str, out_size);
  out[out_size] = '\0';
  return out_size;
}
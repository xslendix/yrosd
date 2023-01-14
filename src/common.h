#ifndef COMMON_H
#define COMMON_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

typedef long long i64;
typedef unsigned long long u64;
typedef int i32;
typedef unsigned int u32;
typedef short i16;
typedef unsigned short u16;
typedef char i8;
typedef unsigned char u8;

#define nullptr NULL

char *trim_string_fast(char *str);
char *to_lower_string(char *str);

u16 random_u16(u16 min, u16 max);

#define BREAKABLE_SCOPE() for (char __scope = 0; __scope == 0; __scope++)

#if defined(__cplusplus)
}
#endif

#endif // COMMON_H


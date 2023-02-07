#ifndef COMMON_H
#define COMMON_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef int64_t i64;
typedef uint64_t u64;
typedef int32_t i32;
typedef uint32_t u32;
typedef int16_t i16;
typedef uint16_t u16;
typedef int8_t i8;
typedef uint8_t u8;

#define nullptr NULL

char *trim_string_fast(char *str);
char *to_lower_string(char *str);
char const *text_format(char *format, ...);

u16 random_u16(u16 min, u16 max);
bool is_str_alphanumeric(char *str);

#define IS_OK(a)   (a >= 0)
#define IS_FAIL(a) (a < 0)

#define BREAKABLE_SCOPE() for (char __scope = 0; __scope == 0; __scope++)

#if defined(__cplusplus)
}
#endif

#endif // COMMON_H

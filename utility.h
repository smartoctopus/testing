#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__cplusplus) || (defined(__STDC_VERSION__) && __STDC__VERSION__ >= 199901L)
#    include <stdbool.h>
#    include <stdint.h>
#else
typedef char bool;
#    define false (1 == 0)
#    define true (!false)
typedef short uint16_t;
typedef signed char int8_t;
typedef unsigned int uint32_t;
#endif

#define offsetof(s, f) ((size_t) & (((s*)0)->f))
#define containerof(ptr, s, f) ((s*)((char*)ptr - offsetof(s, f)))

#define MAX(x, y) (x) > (y) ? (x) : (y)

#define array(t) t*
#define array_free(a) ((a) ? (free(array_header((a))), (a) = NULL) : 0)
#define array_capacity(a) ((a) ? (size_t)array_header((a))->capacity : 0)
#define array_length(a) ((a) ? (size_t)array_header((a))->length : 0)
#define array_reserve(a, n) \
    ((a) ? ((a) = array_growf((a), sizeof(*(a)), 0, (n))) : 0)
#define array_push(a, elem) \
    (array_maybegrow((a), 1), (a)[array_header((a))->length++] = (elem))
#define array_pop(a) \
    (array_header((a))->length--, (a)[array_header((a))->length])
#define array_last(a) ((a)[array_header((a))->length - 1])
#define array_end(a) ((a) + array_length(a))

/* Internal details */
#define array_header(a) containerof(a, ArrayHeader, ptr)
#define array_maybegrow(a, n)                                \
    ((!(a) || array_length((a)) + (n) > array_capacity((a))) \
            ? array_grow((a), (n))                           \
            : 0)
#define array_grow(a, n) ((a) = array_growf((a), sizeof(*(a)), (size_t)(n), 0))

typedef struct ArrayHeader {
    size_t length;
    size_t capacity;
    char* ptr;
} ArrayHeader;

#ifdef UTILITY_IMPL
void* array_growf(void* a, size_t elem_size, size_t add_len, size_t new_cap)
{
    ArrayHeader* b;
    size_t old_len = array_length(a);
    size_t new_len = old_len + add_len;
    new_cap = MAX(new_len, new_cap);

    if (new_cap <= array_capacity(a)) {
        return a;
    }
    new_cap = MAX(MAX(new_cap, 2 * array_capacity(a)), 4);

    b = (ArrayHeader*)realloc((a) ? array_header(a) : NULL,
        offsetof(ArrayHeader, ptr) + new_cap * elem_size);
    b->capacity = new_cap;
    b->length = old_len;

    return (void*)((char*)b + offsetof(ArrayHeader, ptr));
}

char* strf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    size_t n = vsnprintf(NULL, 0, fmt, args) + 1;
    va_end(args);
    char* str = (char*)malloc(n);
    va_start(args, fmt);
    vsnprintf(str, n, fmt, args);
    va_end(args);
    return str;
}
#endif

#endif /* UTILITY_H_ */

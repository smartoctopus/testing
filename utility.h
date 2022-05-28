#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <stdint.h>

#if !defined(__offsetof)
#    define __offsetof(s, f) ((size_t) & (((s*)0)->f))
#    define __containerof(ptr, s, f) ((s*)((char*)ptr - __offsetof(s, f)))
#endif

#if !defined(__MAX)
#    define __MAX(x, y) (x) > (y) ? (x) : (y)
#endif

#if !defined(__array)
#    define __array(t) t*
#    define __array_free(a) ((a) ? (free(__array_header((a))), (a) = NULL) : 0)
#    define __array_capacity(a) ((a) ? (size_t)__array_header((a))->capacity : 0)
#    define __array_length(a) ((a) ? (size_t)__array_header((a))->length : 0)
#    define __array_reserve(a, n) \
        ((a) ? ((a) = __array_growf((a), sizeof(*(a)), 0, (n))) : 0)
#    define __array_push(a, elem) \
        (__array_maybegrow((a), 1), (a)[__array_header((a))->length++] = (elem))
#    define __array_pop(a) \
        (__array_header((a))->length--, (a)[__array_header((a))->length])
#    define __array_last(a) ((a)[__array_header((a))->length - 1])
#    define __array_end(a) ((a) + __array_length(a))

/* Internal details */
#    define __array_header(a) __containerof(a, __ArrayHeader, ptr)
#    define __array_maybegrow(a, n)                                  \
        ((!(a) || __array_length((a)) + (n) > __array_capacity((a))) \
                ? __array_grow((a), (n))                             \
                : 0)
#    define __array_grow(a, n) ((a) = __array_growf((a), sizeof(*(a)), (size_t)(n), 0))

typedef struct __ArrayHeader {
    size_t length;
    size_t capacity;
    char* ptr;
} __ArrayHeader;

#endif

#ifdef UTILITY_IMPL
static void* __array_growf(void* a, size_t elem_size, size_t add_len, size_t new_cap)
{
    __ArrayHeader* b;
    size_t old_len = __array_length(a);
    size_t new_len = old_len + add_len;
    new_cap = __MAX(new_len, new_cap);

    if (new_cap <= __array_capacity(a)) {
        return a;
    }
    new_cap = __MAX(__MAX(new_cap, 2 * __array_capacity(a)), 4);

    b = (__ArrayHeader*)realloc((a) ? __array_header(a) : NULL,
        __offsetof(__ArrayHeader, ptr) + new_cap * elem_size);
    b->capacity = new_cap;
    b->length = old_len;

    return (void*)((char*)b + __offsetof(__ArrayHeader, ptr));
}
#endif

#endif /* UTILITY_H_ */

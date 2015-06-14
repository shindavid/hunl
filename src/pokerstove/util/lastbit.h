#ifndef __LASTBIT_H
#define __LASTBIT_H

#include <cstdint>
#include <pokerstove/util/utypes.h>

inline uint firstbit (uint64_t v)
{
    // __builtin_clz(0) is undefined, so don't do that.
    if (v == 0)
        return 0;
    return 63 - __builtin_clzll(v);
}

inline int lastbit (uint32_t v)
{
    if (v == 0)
        return 0;
    return __builtin_ctz(v);
}

inline int lastbit (uint16_t v)
{
    if (v == 0)
        return 0;
    return __builtin_ctz(v);
}

inline int lastbit (uint64_t v)
{
    if (v == 0)
        return 0;
    return __builtin_ctzll(v);
}

inline int lastbit64 (uint64_t v)
{
    if (v == 0)
        return 0;
    return __builtin_ctzll(v);
}

#endif

#include "hash_func.h"

//===============================================

uint32_t one_hash        (void* data, unsigned int size)
{
    return 1;
}

//-----------------------------------------------

uint32_t first_ascii_hash(void* data, unsigned int size)
{
    return (uint32_t)*((unsigned char*)data);
}

//-----------------------------------------------

uint32_t sizeof_hash     (void* data, unsigned int size)
{
    return size;
}

//-----------------------------------------------

uint32_t ascii_sum_hash  (void* data, unsigned int size)
{
    uint32_t result = 0;

    for (unsigned int counter = 0;
                      counter < size;
                      counter ++)
    {
        result += (uint32_t)*((unsigned char*)data);
    }

    return result;
}

//-----------------------------------------------

uint32_t ror_hash        (void* data, unsigned int size)
{
    return 0;
}

//-----------------------------------------------

uint32_t my_hash         (void* data, unsigned int size)
{
    char* base = (char*)data;

    assert(base);

    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const int r = 24;

    unsigned int h = seed ^ (unsigned int)size;

    const unsigned char* ddata = (const unsigned char*)base;
    unsigned int k = 0;

    while (size >= 4) {

        k = ddata[0];
        k |= (unsigned)(ddata[1] << 8);
        k |= (unsigned)(ddata[2] << 16);
        k |= (unsigned)(ddata[3] << 24);

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        ddata += 4;
        size -= 4;

    }

    switch (size) {
        case 3:
            h ^= (unsigned)(ddata[2] << 16);
            [[fallthrough]];
        case 2:
            h ^= (unsigned)(ddata[1] << 8);
            [[fallthrough]];
        case 1:
            h ^= ddata[0];
            h *= m;
            [[fallthrough]];
        default:;
    }

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

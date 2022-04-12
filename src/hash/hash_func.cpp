#include "hash_func.h"

//===============================================

uint32_t one_hash(void* data, unsigned int size)
{
    return 1;
}

//-----------------------------------------------

uint32_t first_ascii_hash(void* data, unsigned int size)
{
    return (uint32_t)*((unsigned char*)data);
}

//-----------------------------------------------

uint32_t sizeof_hash(void* data, unsigned int size)
{
    return size;
}

//-----------------------------------------------

uint32_t ascii_sum_hash(void* data, unsigned int size)
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

uint32_t ror_hash(void* data, unsigned int size)
{
    unsigned char* string = (unsigned char*)data;
    uint32_t return_value = string[0];

    for (unsigned int counter = 1;
                      counter < size;
                      counter++)
    {
        return_value = ( (return_value << 1) | (return_value >> 31) ) ^ string[counter];
    }

    return return_value;
}

//-----------------------------------------------

uint32_t my_hash(void* data, unsigned int size)
{
   unsigned int byte, crc, mask;   
   unsigned char* string = (unsigned char*)data; 

   crc = 0xFFFFFFFF;
   for (unsigned int counter = 0; 
                     counter < size;
                     counter++) 
    {
        byte = string[counter];
        crc = crc ^ byte;
        for (int j = 7; j >= 0; j--) 
        {
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
    }    

   return ~crc;
}

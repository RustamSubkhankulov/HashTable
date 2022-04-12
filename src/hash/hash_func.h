#pragma once 

//===============================================

#include <stdint.h>

#include "hash.h"

//===============================================

//Testing hash functions

uint32_t one_hash        (void* data, unsigned int size);

uint32_t first_ascii_hash(void* data, unsigned int size);

uint32_t sizeof_hash     (void* data, unsigned int size);

uint32_t ascii_sum_hash  (void* data, unsigned int size);

uint32_t rol_hash        (void* data, unsigned int size);

uint32_t my_hash         (void* data, unsigned int size);

//===============================================
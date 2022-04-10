#pragma once 

#include "hash_conf.h"
#include "../logs/errors_and_logs.h"
#include "../list/list.h"
#include "../hamlet/hamlet.h"

//===============================================

#ifdef HASH_LOGS

    #define hash_log_report() \
            log_report()

#else 

    #define hash_log_repotr() ""

#endif 

//--------------------------------------------------------------

#ifdef HASH_TABLE_VALID_CHECK

    #define HASH_TABLE_VALID(hash_table_ptr)                    \
                                                                \
            do                                                  \
            {                                                   \
                if (hash_table_validator(hash_table_ptr) == -1) \
                    return -1;                                  \
                                                                \
            } while(0);                                         

#else 

    #define HASH_TABLE_VALID(hash_tabel_ptr) ""

#endif 

//==============================================================

struct Hash_table 
{
    List** data;

    unsigned int size;
    unsigned int capacity;

    uint32_t (*hash_func) (void*, unsigned int);
};

//===============================================

#define HASH_TABLE_PTR_CHECK(hash_table_ptr)    \
                                                \
    do                                          \
    {                                           \
        if (!hash_table_ptr)                    \
        {                                       \
            error_report(INV_HASH_TABLE_PTR);   \
            return -1;                          \
        }                                       \
                                                \
    } while(0); 

//===============================================

//Testing hash functions

uint32_t one_hash        (void* data, unsigned int size);

uint32_t first_ascii_hash(void* data, unsigned int size);

uint32_t sizeof_hash     (void* data, unsigned int size);

uint32_t ascii_sum_hash  (void* data, unsigned int size);

uint32_t ror_hash        (void* data, unsigned int size);

uint32_t my_hash         (void* data, unsigned int size);

//===============================================

int _hash_table_set_hash_func(Hash_table* hash_table, uint32_t (*hash_func) (void*, unsigned int) FOR_LOGS(, LOG_PARAMS));

//===============================================

int _hash_table_ctor(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS));

int _hash_table_dtor(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS));

int _hash_table_search(Hash_table* hash_table, elem_t elem, unsigned int size, List** list 
                                                                   FOR_LOGS(, LOG_PARAMS));

int _hash_table_insert(Hash_table* hash_table, elem_t elem, List* list 
                                               FOR_LOGS(, LOG_PARAMS));

int _hash_table_delete(Hash_table* hash_table, unsigned int index, List* list 
                                                      FOR_LOGS(, LOG_PARAMS));

int _hash_table_smart_insert(Hash_table* hash_table, elem_t elem, unsigned int size,
                                                List** list FOR_LOGS(, LOG_PARAMS));

int _hash_table_compare_hash_func(Hash_table* hash_table, const char* out, const char* src 
                                                                  FOR_LOGS(, LOG_PARAMS));

//===============================================

#define hash_table_compare_hash_func(table, out, src) \
       _hash_table_compare_hash_func(table, out, src FOR_LOGS(, LOG_ARGS))

#define hash_table_set_hash_func(hash_table, hash_func) \
       _hash_table_set_hash_func(hash_table, hash_func FOR_LOGS(, LOG_ARGS))

#define hash_table_ctor(hash_table) \
       _hash_table_ctor(hash_table FOR_LOGS(, LOG_ARGS))

#define hash_table_dtor(hash_table) \
       _hash_table_dtor(hash_table FOR_LOGS(, LOG_ARGS))

#define hash_table_search(hash_table, elem, size, list) \
       _hash_table_search(hash_table, elem, size, list FOR_LOGS(, LOG_ARGS))

#define hash_table_insert(hash_table, elem, list) \
       _hash_table_insert(hash_table, elem, list FOR_LOGS(, LOG_ARGS))

#define hash_table_smart_insert(hash_table, elem, size, list) \
       _hash_table_smart_insert(hash_table, elem, size, list FOR_LOGS(, LOG_ARGS))

#define hash_table_delete(hash_table, index, list) \
       _hash_table_delete(hash_table, index, list FOR_LOGS(, LOG_ARGS))

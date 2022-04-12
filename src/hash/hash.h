#pragma once 

#include "hash_conf.h"
#include "hash_func.h"
#include "../logs/errors_and_logs.h"
#include "../list/list.h"
#include "../hamlet/hamlet.h"

//===============================================

#ifdef HASH_LOGS

    #ifdef HASH_ONLY_DUMPS

        #define hash_log_report() ""

    #else 

        #define hash_log_report() \
                log_report()

    #endif 

#else 

    #define hash_log_report() ""

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
    List* data;

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

// Performs stress test: insert all unique words from file to hash_table, searches them and then
// deletes them one by one from hash table

int _hash_table_stress_test(const char* src, uint32_t (*hash_func) (void*, unsigned int) 
                                                                FOR_LOGS(, LOG_PARAMS));

// Generates 6 graphs for 6 different types of hash functions to compare their characteristics

int _hash_table_compare_hash_func(const char* out, const char* src 
                                           FOR_LOGS(, LOG_PARAMS));

// Dumps info about hash_tables and lists in html

int _hash_table_dump(Hash_table* hash_table, FILE* output FOR_LOGS(, LOG_PARAMS));

//===============================================

int _hash_table_set_hash_func(Hash_table* hash_table, uint32_t (*hash_func) (void*, unsigned int) 
                                                                         FOR_LOGS(, LOG_PARAMS));

int _hash_table_ctor(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS));

int _hash_table_dtor(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS));

int _hash_table_search(Hash_table* hash_table, elem_t elem, unsigned int size, 
                                          List** list FOR_LOGS(, LOG_PARAMS));

int _hash_table_insert(Hash_table* hash_table, elem_t elem, List* list 
                                               FOR_LOGS(, LOG_PARAMS));

int _hash_table_delete(Hash_table* hash_table, unsigned int index, List* list 
                                                      FOR_LOGS(, LOG_PARAMS));

int _hash_table_smart_insert(Hash_table* hash_table, elem_t elem, unsigned int size,
                                                List** list FOR_LOGS(, LOG_PARAMS));

//===============================================

#define hash_table_stress_test(src, hash_func) \
       _hash_table_stress_test(src, hash_func FOR_LOGS(, LOG_ARGS))


#define hash_table_dump(hash_table, output) \
       _hash_table_dump(hash_table, output FOR_LOGS(, LOG_ARGS))

#define hash_table_compare_hash_func(out, src) \
       _hash_table_compare_hash_func(out, src FOR_LOGS(, LOG_ARGS))

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

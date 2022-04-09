#include "hash.h"

//===============================================

static int _hash_table_validator(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS));

static int _hash_table_dump(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS));

static int _hash_table_byte_check(Hash_table* hash_table, unsigned char check_value FOR_LOGS(, LOG_PARAMS));

static int _hash_table_byte_fill (Hash_table* hash_table, unsigned char fill_value  FOR_LOGS(, LOG_PARAMS));

static int _hash_table_reallocate(Hash_table* hash_table, unsigned int New_cap FOR_LOGS(, LOG_PARAMS));

//===============================================

#define hash_table_validator(hash_table) \
       _hash_table_validator(hash_table FOR_LOGS(, LOG_ARGS))

#define hash_table_dump(hash_table) \
       _hash_table_dump(hash_table FOR_LOGS(, LOG_ARGS))

#define hash_table_byte_check(hash_table) \
       _hash_table_byte_check(hash_table FOR_LOGS(, LOG_ARGS))

#define hash_table_byte_fill(hash_table) \
       _hash_table_byte_fill(hash_table FOR_LOGS(, LOG_ARGS))

//===============================================

static int _hash_table_reallocate(Hash_table* hash_table, unsigned int new_cap FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);
    

    if (new_cap )
}

static int _hash_table_validator(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);

    return 0;
}

//-----------------------------------------------

static int _hash_table_dump(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
}

//-----------------------------------------------

int _hash_table_set_hash_func(Hash_tabel* hash_table, uint32_t (*hash_func) (void*, unsigned int) FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);
}

//-----------------------------------------------

static int _hash_table_byte_check(Hash_table* hash_table, unsigned char check_value FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);

    for (unsigned int counter = 0;
                      counter < sizeof(Hash_table); counter++)
    {
        if ((unsigned char*) hash_table + counter != check_value)
            return 0;
    }
                      
    return 1;     
}

//-----------------------------------------------

static int _hash_table_byte_fill (Hash_table* hash_table, unsigned char fill_value  FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);

    for (unsigned int counter = 0;
                      counter < sizeof(Hash_table); counter++)
    {
        (unsigned char*) hash_table + counter = fill_value
    }
                      
    return 0;   
}

//-----------------------------------------------

int _hash_table_ctor(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);

    int is_clear    = hash_table_byte_check(hash_table, 0x00);
    int is_poisoned = hash_table_byte_check(hash_table, Hash_table_poison_value);

    if (is_clear == 0 && is_poisoned == 0)
    {
        error_report(HASH_TABLE_UNPREP);
        return -1;
    }

    if (is_poisoned)
    {
        int ret_val = hash_table_byte_fill(hash_table, 0x00);
        if (ret_val == -1)
            return -1;
    }

    if (Hash_table_init_cap > Hash_table_max_cap)
    {
        error_report(HASH_TABLE_INV_CONF_SIZE);
        return -1;
    }

    hash_table->size = 0;
    
    int ret_val = hash_table_reallocate(hash_table, Hash_tabel_init_cap) 
    if (ret_val == -1)
        return -1;

    HASH_TABLE_VALID(hash_table);
    return 0;
}

//-----------------------------------------------



//-----------------------------------------------

int _hash_table_dtor(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);
}

//-----------------------------------------------

int _hash_table_search(Hash_table* hash_table, elem_t elem, List* list FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);
}

//-----------------------------------------------

int _hash_table_insert(Hash_table* hash_table, elem_t elem FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);
}

//-----------------------------------------------

int _hash_table_delete(Hash_table* hash_table, unsigned int index, List* list FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);
}

//-----------------------------------------------

int _hash_table_testing(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);
}

//-----------------------------------------------

int _hash_table_flush_stats(Hash_table* hash_table, FILE* stat_file FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);
}
#include "hash.h"
#include "../general/general.h"

//===============================================

static int _hash_table_validator(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS));

static int _hash_table_dump(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS));

static int _hash_table_byte_check(Hash_table* hash_table, unsigned char check_value FOR_LOGS(, LOG_PARAMS));

static int _hash_table_byte_fill (Hash_table* hash_table, unsigned char fill_value  FOR_LOGS(, LOG_PARAMS));

static int _hash_table_reallocate(Hash_table* hash_table, unsigned int New_cap FOR_LOGS(, LOG_PARAMS));

static int _hash_table_increase(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS))

//===============================================

#define hash_table_increase(hash_table) \
       _hash_table_increase(hash_table FOR_LOGS(, LOG_ARGS))

#define hash_table_validator(hash_table) \
       _hash_table_validator(hash_table FOR_LOGS(, LOG_ARGS))

#define hash_table_dump(hash_table) \
       _hash_table_dump(hash_table FOR_LOGS(, LOG_ARGS))

#define hash_table_byte_check(hash_table) \
       _hash_table_byte_check(hash_table FOR_LOGS(, LOG_ARGS))

#define hash_table_byte_fill(hash_table) \
       _hash_table_byte_fill(hash_table FOR_LOGS(, LOG_ARGS))

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
    return 0;
}

//===============================================

static int _hash_table_reallocate(Hash_table* hash_table, unsigned int new_cap FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);
    
    if (new_cap < hash_table->capacity)
    {
        error_report(HASH_TABLE_CAP_DECR);
        return -1;
    }

    if (hash_table->capacity == 0)
    {
        hash_table->data = (List**) calloc((size_t) new_cap, sizeof(List*));
    }

    else 
    {
        hash_table->data = (List**) my_recalloc(hash_table->data, new_cap, 
                                                hash_table->capacity, sizeof(List*));
    }

    if (hash->table == NULL)
    {
        error_report(CANNOT_ALLOCATE_MEM);
        return -1;
    }

    hash_table->capacity = new_cap;

    return 0;
}

//-----------------------------------------------

static int _hash_table_validator(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);

    int errors_ct = 0;

    if (hash_table->size != 0 && hash_table->capacity == 0)
    {
        error_report(HASH_TABLE_INV_CAP_LESS_SIZE);
        errors_ct += 1;
    }

    if (hash_table->capacity != 0 && hash_table->data == NULL)
    {
        error_report(HASH_TABLE_INV_DATA_PTR);
        errors_ct += 1;
    }

    #ifdef HASH_TABLE_DUMP

        if (hash_table->data != NULL)
        {
            int ret_val = hash_table_dump(hash_table);
            if (ret_val == -1)
                return -1;
        }

    #endif 

    if (errors_ct != 0 )
        return -1;

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

    if (hash_func == NULL)
    {
        error_report(INV_HASH_FUNC_PTR);
        return -1;
    }

    hash_table->hash_func = hash_func;

    return 0;
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

    for (unsigned int counter = 0; 
                      counter < hash_table->capacity;
                      counter++)
    {
        hash_table->data[counter] = (List*) calloc(1, sizeof(List));
        if (!hash_table->data[counter])
        {
            error_report(CANNOT_ALLOCATE_MEM;
            return -1;
        }

        if (list_ctor(hash_table->data[counter]) == -1)
            return -1;
    }

    HASH_TABLE_VALID(hash_table);
    return 0;
}

//-----------------------------------------------

int _hash_table_dtor(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);

    free(hash_table->data);

    int ret_val = hash_tabel_byte_fill(hash_table, Hash_table_poison_value);
    if (ret_val == -1)
        return -1;
    
    return 0;
}

//-----------------------------------------------

int _hash_table_search(Hash_table* hash_table, elem_t elem, List** list FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);

    if (hash_table->size == 0)
    {
        list = NULL;
        return 0;
    }

    uint32_t hash_value = (hash_table->hash_func) ((void*) &elem, sizeof(elem));
    *list = hash_table->data[hash_value % hash_table->capacity];

    return list_search(*list, elem);
}

//-----------------------------------------------

static int _hash_table_increase(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);

    float fill_factor = hash_table->size / hash_table->capacity;
    unsigned int prev_cap = hash_table->capacity;

    if (fill_factor > Resize_fill_factor)
    {
        int ret_val = hash_table_reallocate(hash_table, hash_tabel->capacity * 2);
        if (ret_val == -1)
            return -1;
    }

    for (unsigned int counter = prev_cap;
                      counter < hash_table->capacity;
                      counter ++)
    {
        hash_table->data[counter] = (List*) calloc(1, sizeof(List));        //вынос кода 
        if (!hash_table->data[counter])                                     // в ctor тоже 
        {
            error_report(CANNOT_ALLOCATE_MEM);
            return -1;
        }

        if (list_ctor(hash_tabel->data[counter]) == -1)
            return -1;
    }

    return 0;
}
//-----------------------------------------------

int _hash_table_insert(Hash_table* hash_table, elem_t elem, List* list FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);

    #ifdef HASH_TABLE_RESIZE

        int ret_val = hash_table_increase(hash_table);
        if (ret_val == -1)
            return -1;
    #endif  

    hash_table->size += 1;

    return list_push_back(list, elem);
}

//-----------------------------------------------

int _hash_table_delete(Hash_table* hash_table, unsigned int index, List* list FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);

    hash_table->size -= 1;

    int err = 0;
    list_pop_by_index(list, index, &err);

    return err;
}

//-----------------------------------------------

int _hash_table_testing(Hash_table* hash_table, uint32_t (*hash_func) (void*, unsigned int) FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);

    if (hash_table_ctor(hash_tabel) == -1)
        return -1;

    if (hash_table_set_hash_func(hash_tabel, hash_func) == -1)
        return -1;

    
}

//-----------------------------------------------

int _hash_table_flush_stats(Hash_table* hash_table, FILE* stat_file FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);
}
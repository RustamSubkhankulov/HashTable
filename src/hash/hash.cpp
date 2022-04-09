#include <stdlib.h>

//===============================================

#include "hash.h"
#include "../general/general.h"
#include "../list/list.h"

//===============================================

static int _hash_table_validator(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS));

static int _hash_table_dump(Hash_table* hash_table, FILE* output FOR_LOGS(, LOG_PARAMS));

static int _hash_table_byte_check(Hash_table* hash_table, unsigned char check_value FOR_LOGS(, LOG_PARAMS));

static int _hash_table_byte_fill (Hash_table* hash_table, unsigned char fill_value  FOR_LOGS(, LOG_PARAMS));

static int _hash_table_reallocate(Hash_table* hash_table, unsigned int New_cap FOR_LOGS(, LOG_PARAMS));

static int _hash_table_increase(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS));

static int _hash_table_dump_lists(Hash_table* hash_table, FILE* output FOR_LOGS(, LOG_PARAMS));

//===============================================

#define hash_table_reallocate(hash_table, new_cap) \
       _hash_table_reallocate(hash_table, new_cap FOR_LOGS(, LOG_ARGS))

#define hash_table_dump_lists(hash_table, output) \
       _hash_table_dump_lists(hash_table, output FOR_LOGS(, LOG_ARGS))

#define hash_table_increase(hash_table) \
       _hash_table_increase(hash_table FOR_LOGS(, LOG_ARGS))

#define hash_table_validator(hash_table) \
       _hash_table_validator(hash_table FOR_LOGS(, LOG_ARGS))

#define hash_table_dump(hash_table, output) \
       _hash_table_dump(hash_table, output FOR_LOGS(, LOG_ARGS))

#define hash_table_byte_check(hash_table, byte) \
       _hash_table_byte_check(hash_table, byte FOR_LOGS(, LOG_ARGS))

#define hash_table_byte_fill(hash_table, byte) \
       _hash_table_byte_fill(hash_table, byte FOR_LOGS(, LOG_ARGS))

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

//===============================================

static int _hash_table_reallocate(Hash_table* hash_table, unsigned int new_cap FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    
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

    if (hash_table->data == NULL)
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

    if (hash_table->data == NULL)
    {
        error_report(HASH_TABLE_INV_DATA_PTR);
        errors_ct += 1;
    }

    else
    {
        for (unsigned int counter = 0;
                          counter < hash_table->capacity;
                          counter ++)
        {
            if (!hash_table->data[counter])
            {
                error_report(HASH_T_NULL_LIST_PTR);
                errors_ct += 1;
            }

            else 
            {
                if (list_validator(hash_table->data[counter]) == -1)
                    return -1;                                //вынос кода
            }
        }
    } 

    #ifdef HASH_TABLE_DUMP

        if (hash_table->data != NULL)
        {
            if (hash_table_dump(hash_table, logs_file) == -1)
                return -1;
        }

    #endif 

    if (errors_ct != 0 )
        return -1;

    return 0;
}

//-----------------------------------------------

static int _hash_table_dump(Hash_table* hash_table, FILE* output FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    FILE_PTR_CHECK(output);

    fprintf(output, "\n <table style = \"border-collapse: collapse;"
                                        "border: 3px solid black\" "
                                        "class = \"table\">\n");

    fprintf(output, "<caption style=\"border-collapse: collapse;   "
                                     "background-color: blue; "
                                     "border: 2px solid black;\">  "
                                     "<b>Hash table structure.</b> "
                                                       "</caption>");

    fprintf(output, "<tr><td>Address</td><td> <%p></td></tr>\n", 
                                                    hash_table);


    fprintf(output, "<tr><td> Capacity </td><td>%u</td></tr>\n", 
                                          hash_table->capacity);

    fprintf(output, "<tr><td> Size </td><td>%u</td></tr>\n", 
                                          hash_table->size);

    fprintf(output, "<tr><td>Data array address</td><td><%p></td></tr>\n", 
                                                        hash_table->data);

    fprintf(output, "<tr><td> Hash func address </td><td><%p></td></tr>\n", 
                                                    hash_table->hash_func);
    fprintf(output, "\n</table>\n");

    return hash_table_dump_lists(hash_table, output);
}

//-----------------------------------------------

static int _hash_table_dump_lists(Hash_table* hash_table, FILE* output FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);

    fprintf(output, "<style> td\n { padding: 7px;"
                                  " border: 1px solid black;"
                                  " border-collapse: collapse;}\n</style>");

    fprintf(output, "<table width = \" 100%% \" "
                           "cellspacing=\"0\" "
                           "cellpadding=\"4\" "
                           "border = \"5\" "
                           "style = \" "
                           "padding: 15px; "
                           "background-color: blue;>\"\n");

    fprintf(output, "<tr><td><b>Index</b></td>");

    for (unsigned counter = 0; counter < hash_table->capacity; counter++) 
        fprintf(output, "<td><b> %u </b></td>", counter);
    
    fprintf(output, "</tr>");

    fprintf(output, "<tr><td><b>Data</b></td>");

    for (unsigned counter = 0; counter < hash_table->capacity; counter++) 
        fprintf(output, "<td> %p </td>", hash_table->data[counter]);

    fprintf(output, "</tr>");

    fprintf(output, "</table>");

    for (unsigned int counter = 0;
                      counter < hash_table->capacity;
                      counter ++)
    {
        if (hash_table->data[counter] != NULL)
        {
            if (list_dump(hash_table->data[counter], output) == -1)
                return -1;
        }
    }

    return 0;
}
//-----------------------------------------------

int _hash_table_set_hash_func(Hash_table* hash_table, uint32_t (*hash_func) (void*, unsigned int) FOR_LOGS(, LOG_PARAMS))
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
        if ( *((unsigned char*) hash_table + counter) != check_value)
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
        *((unsigned char*) hash_table + counter) = fill_value;
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
        if (hash_table_byte_fill(hash_table, 0x00) == -1)
            return -1;
    }

    if (Hash_table_init_cap > Hash_table_max_cap)
    {
        error_report(HASH_TABLE_INV_CONF_SIZE);
        return -1;
    }

    hash_table->size = 0;

    if (Hash_table_init_cap == 0)
    {
        error_report(HASH_T_ZERO_INIT_CAP);
        return -1;
    }
    
    if (hash_table_reallocate(hash_table, Hash_table_init_cap) == -1)
        return -1;

    for (unsigned int counter = 0; 
                      counter < hash_table->capacity;
                      counter++)
    {
        hash_table->data[counter] = (List*) calloc(1, sizeof(List));
        if (!hash_table->data[counter])
        {
            error_report(CANNOT_ALLOCATE_MEM);
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

    for (unsigned int counter = 0;
                      counter < hash_table->capacity;
                      counter ++ )
    {
        if (list_dtor(hash_table->data[counter]) == -1)
            return -1;

        free(hash_table->data[counter]);
    }

    free(hash_table->data);

    if (hash_table_byte_fill(hash_table, Hash_table_poison_value) == -1)
        return -1;

    return 0;
}

//-----------------------------------------------

int _hash_table_search(Hash_table* hash_table, elem_t elem, List** list FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);

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
        if (hash_table_reallocate(hash_table, hash_table->capacity * 2) == -1)
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

        if (list_ctor(hash_table->data[counter]) == -1)
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

        if (hash_table_increase(hash_table) == -1)
            return -1;  

    #else 

        if (hash_table->size == hash_table->capacity)
        {
            error_report(HASH_T_MAX_SIZE_REACHED);
            return -1;
        }

    #endif

    hash_table->size += 1;

    return list_push_back(list, elem);
}

//-----------------------------------------------

int _hash_table_smart_insert(Hash_table* hash_table, elem_t elem, List** list FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);

    int index = hash_table_search(hash_table, elem, list);
    if (index == -1)
        return -1;

    if (index != ELEMENT_NOT_FOUND)
        return index;

    return hash_table_insert(hash_table, elem, *list); 
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

    if (hash_table_ctor(hash_table) == -1)
        return -1;

    if (hash_table_set_hash_func(hash_table, hash_func) == -1)
        return -1;

    List* list = NULL;
    int index = 0;

    // index = hash_table_search(hash_table, "privet", &list);
    // if (index == -1)
    //     return -1;

    // index = hash_table_insert(hash_table, "privet", list);
    // if (index == -1)
    //     return -1;

    // index = hash_table_search(hash_table, "privet", &list);
    // if (index == -1)
    //     return -1;

    // index = hash_table_insert(hash_table, "privet", list);
    // if (index == -1)
    //     return -1;

    // index = hash_table_search(hash_table, "privet", &list);
    // if (index == -1)
    //     return -1;

    index = hash_table_smart_insert(hash_table, "privet", &list);
    if (index == -1)
        return -1;

    index = hash_table_smart_insert(hash_table, "privet", &list);
    if (index == -1)
        return -1;

    index = hash_table_smart_insert(hash_table, "privet", &list);
    if (index == -1)
        return -1;

    int ret_val = hash_table_delete(hash_table, index, list);
    if (ret_val == -1)
        return -1;

    if (hash_table_dtor(hash_table) == -1)
        return -1;

    return 0;
}

//-----------------------------------------------

// int _hash_table_flush_stats(Hash_table* hash_table, FILE* stat_file FOR_LOGS(, LOG_PARAMS))
// {
//     hash_log_report();
//     HASH_TABLE_PTR_CHECK(hash_table);
//     HASH_TABLE_VALID(hash_table);
//     FILE_PTR_CHECK(stat_file);

//     for (unsigned int counter = 0;
//                       counter < hash_table->capacity;
//                       counter ++)
//     {
//         fprintf(stat_file, "%d; %d; ", counter, (hash_table->data[counter])->size);
//     }

//     fprintf(stat_file, "\n");

// }
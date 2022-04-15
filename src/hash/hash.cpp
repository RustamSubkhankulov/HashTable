#include <stdlib.h>
#include <time.h>
#include <string.h>

//===============================================

#include "hash.h"
#include "../general/general.h"
#include "../list/list.h"

//===============================================

extern "C" int list_search_asm(struct List* list, elem_t elem); 

//===============================================

static int _hash_table_validator(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS));

static int _hash_table_byte_check(Hash_table* hash_table, unsigned char check_value 
                                                           FOR_LOGS(, LOG_PARAMS));

static int _hash_table_byte_fill (Hash_table* hash_table, unsigned char fill_value  
                                                          FOR_LOGS(, LOG_PARAMS));

static int _hash_table_reallocate(Hash_table* hash_table, unsigned int New_cap 
                                                       FOR_LOGS(, LOG_PARAMS));

static int _hash_table_test_hash_func(FILE* out, const Hamlet* hamlet, 
                                      uint32_t (*hash_func) (void*, unsigned int) 
                                                         FOR_LOGS(, LOG_PARAMS));

static int _hash_table_flush_stats(Hash_table* hash_table, FILE* out 
                                             FOR_LOGS(, LOG_PARAMS));

static int _hash_table_stress_test_perf(const Hamlet* hamlet, Hash_table* hash_table 
                                                              FOR_LOGS(, LOG_PARAMS));

#ifdef HASH_TABLE_LIST_DUMPS

    static int _hash_table_dump_lists(Hash_table* hash_table, FILE* output 
                                                   FOR_LOGS(, LOG_PARAMS));
#endif 

#ifdef HASH_TABLE_INCREASE

    static int _hash_table_increase(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS));

#endif 

//===============================================

#define hash_table_reallocate(hash_table, new_cap) \
       _hash_table_reallocate(hash_table, new_cap FOR_LOGS(, LOG_ARGS))

#define hash_table_dump_lists(hash_table, output) \
       _hash_table_dump_lists(hash_table, output FOR_LOGS(, LOG_ARGS))

#define hash_table_increase(hash_table) \
       _hash_table_increase(hash_table FOR_LOGS(, LOG_ARGS))

#define hash_table_validator(hash_table) \
       _hash_table_validator(hash_table FOR_LOGS(, LOG_ARGS))

#define hash_table_byte_check(hash_table, byte) \
       _hash_table_byte_check(hash_table, byte FOR_LOGS(, LOG_ARGS))

#define hash_table_byte_fill(hash_table, byte) \
       _hash_table_byte_fill(hash_table, byte FOR_LOGS(, LOG_ARGS))

#define hash_table_flush_stats(hash_table, file) \
       _hash_table_flush_stats(hash_table, file FOR_LOGS(, LOG_ARGS))

#define hash_table_test_hash_func(out, hamlet, hash_func) \
       _hash_table_test_hash_func(out, hamlet, hash_func FOR_LOGS(, LOG_ARGS))

#define hash_table_stress_test_perf(hamlet, hash_table) \
       _hash_table_stress_test_perf(hamlet, hash_table FOR_LOGS(, LOG_ARGS))

//===============================================

static int _hash_table_reallocate(Hash_table* hash_table, unsigned int new_cap 
                                                       FOR_LOGS(, LOG_PARAMS))
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
        hash_table->data = (List*) calloc((size_t) new_cap, sizeof(List));
    }

    else 
    {
        hash_table->data = (List*) my_recalloc(hash_table->data, new_cap, 
                                               hash_table->capacity, sizeof(List));
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

    #ifdef HASH_TABLE_LIST_VALID_CHECK

        for (unsigned int counter = 0;
                            counter < hash_table->capacity;
                            counter ++)
        {
            int ret_val = list_validator(&hash_table->data[counter]);
            if (ret_val == -1) return -1;                    
        }
        
    #endif 

    #ifdef HASH_TABLE_DUMP

        if (hash_table->data != NULL)
        {
            int ret_val = hash_table_dump(hash_table, logs_file);
            if (ret_val == -1) return -1;
        }

    #endif 

    if (errors_ct != 0 )
        return -1;

    return 0;
}

//-----------------------------------------------

int _hash_table_dump(Hash_table* hash_table, FILE* output FOR_LOGS(, LOG_PARAMS))
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

    #ifdef HASH_TABLE_LIST_DUMPS

        return hash_table_dump_lists(hash_table, output);

    #else 

        return 0;

    #endif 
}

//-----------------------------------------------

#ifdef HASH_TABLE_LIST_DUMPS

static int _hash_table_dump_lists(Hash_table* hash_table, FILE* output 
                                               FOR_LOGS(, LOG_PARAMS))
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
        fprintf(output, "<td> %p </td>", &hash_table->data[counter]);

    fprintf(output, "</tr>");

    fprintf(output, "</table>");

    for (unsigned int counter = 0;
                      counter < hash_table->capacity;
                      counter ++)
    {
        int ret_val = list_dump(&hash_table->data[counter], output);
        if (ret_val == -1) return -1;
    }

    return 0;
}

#endif

//-----------------------------------------------

int _hash_table_set_hash_func(Hash_table* hash_table, 
                              uint32_t (*hash_func) (void*, unsigned int) 
                                                  FOR_LOGS(, LOG_PARAMS))
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

static int _hash_table_byte_check(Hash_table* hash_table, unsigned char check_value 
                                                            FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);

    for (unsigned int counter = 0;
                      counter < sizeof(Hash_table); 
                      counter++)
    {
        if ( *((unsigned char*) hash_table + counter) != check_value)
            return 0;
    }
                      
    return 1;     
}

//-----------------------------------------------

static int _hash_table_byte_fill (Hash_table* hash_table, unsigned char fill_value  
                                                            FOR_LOGS(, LOG_PARAMS))
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
        int ret_val = hash_table_byte_fill(hash_table, 0x00);
        if (ret_val == -1) return -1;
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
    
    int ret_val = hash_table_reallocate(hash_table, Hash_table_init_cap);
    if (ret_val == -1) return -1; 

    for (unsigned int counter = 0; 
                      counter < hash_table->capacity;
                      counter++)
    {
        ret_val = list_ctor(&hash_table->data[counter]);
        if (ret_val == -1) return -1; 
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
        int ret_val = list_dtor(&hash_table->data[counter]);
        if (ret_val == -1) return -1; 
    }

    free(hash_table->data);

    int ret_val = hash_table_byte_fill(hash_table, Hash_table_poison_value);
    if (ret_val == -1) return -1;

    return 0;
}

//-----------------------------------------------

int _hash_table_search(Hash_table* hash_table, elem_t elem, unsigned int size, 
                                           List** list FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);

    uint32_t hash_value = (hash_table->hash_func) ((void*) elem, size);
    *list = &hash_table->data[hash_value % hash_table->capacity];

    return list_search_asm(*list, elem);
}

//-----------------------------------------------

#ifdef HASH_TABLE_INCREASE

static int _hash_table_increase(Hash_table* hash_table FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);

    float fill_factor = hash_table->size / hash_table->capacity;
    unsigned int prev_cap = hash_table->capacity;

    if (fill_factor > Resize_fill_factor)
    {

        int ret_val = hash_table_reallocate(hash_table, hash_table->capacity * 2);
        if (ret_val == -1) return -1;
    }

    for (unsigned int counter = prev_cap;
                      counter < hash_table->capacity;
                      counter ++)
    {
        int ret_val = list_ctor(&hash_table->data[counter]);
        if (ret_val == -1) return -1; 
    }

    return 0;
}

#endif 

//-----------------------------------------------

int _hash_table_insert(Hash_table* hash_table, elem_t elem, List* list 
                                               FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);

    #ifdef HASH_TABLE_RESIZE

        int is_ok_inc = hash_table_increase(hash_table);
        if (is_ok_inc == -1) return -1; 

    #endif

    hash_table->size += 1;

    return list_push_back(list, elem);
}

//-----------------------------------------------

int _hash_table_smart_insert(Hash_table* hash_table, elem_t elem, unsigned int size, 
                                                 List** list FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);

    //printf("\n searching %s \n", elem);
    //fflush(stdout);

    int index = hash_table_search(hash_table, elem, size, list);
    if (index == -1)
        return -1;

    if (index != ELEMENT_NOT_FOUND)
        return index;

    return hash_table_insert(hash_table, elem, *list); 
}

//-----------------------------------------------

int _hash_table_delete(Hash_table* hash_table, unsigned int index, List* list 
                                                      FOR_LOGS(, LOG_PARAMS))
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

int _hash_table_compare_hash_func(const char* out, const char* src FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    FILENAME_CHECK(out);
    FILENAME_CHECK(src);

    FILE* out_file_ptr = open_file(out, "wb");
    if (!out_file_ptr) return -1;

    Hamlet hamlet = { 0 };
    int ret_val = hamlet_init(&hamlet, src);
    if (ret_val == -1) return -1;

    ret_val = hash_table_test_hash_func(out_file_ptr, &hamlet, one_hash);
    if (ret_val == -1) return -1;

    ret_val = hash_table_test_hash_func(out_file_ptr, &hamlet, first_ascii_hash);
    if (ret_val == -1) return -1;

    ret_val = hash_table_test_hash_func(out_file_ptr, &hamlet, sizeof_hash);
    if (ret_val == -1) return -1;

    ret_val = hash_table_test_hash_func(out_file_ptr, &hamlet, ascii_sum_hash);
    if (ret_val == -1) return -1;

    ret_val = hash_table_test_hash_func(out_file_ptr, &hamlet, crc32_hash);
    if (ret_val == -1) return -1;

    ret_val = hash_table_test_hash_func(out_file_ptr, &hamlet, my_hash);
    if (ret_val == -1) return -1;

    ret_val = close_file(out_file_ptr);
    if (ret_val == -1) return -1;

    ret_val = hamlet_destruct(&hamlet);
    if (ret_val == -1) return -1;

    system("python3 python/hist.py text_files/res.txt");

    return 0;
}

//-----------------------------------------------

static int _hash_table_test_hash_func(FILE* out, const Hamlet* hamlet, 
                                      uint32_t (*hash_func) (void*, unsigned int) FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    FILE_PTR_CHECK(out);

    if (!hamlet)
    {
        error_report(INV_HAMLET_STRUCTURE_PTR);
        return -1;
    }

    if (!hash_func)
    {
        error_report(INV_HASH_FUNC_PTR);
        return -1;
    }

    Hash_table hash_table = { 0 };
    int ret_val = 0;

    ret_val = hash_table_ctor(&hash_table);
    if (ret_val == -1) return -1;

    ret_val = hash_table_set_hash_func(&hash_table, hash_func);
    if (ret_val == -1) return -1;

    List* list = NULL;

    for (unsigned int counter = 0;
                      counter < hamlet->number;
                      counter++)
    {

        ret_val = hash_table_smart_insert(&hash_table, hamlet->tokens[counter].data, 
                                                       hamlet->tokens[counter].len, 
                                                       &list);
        if (ret_val == -1)
            return -1;
    }

    ret_val = hash_table_flush_stats(&hash_table, out);
    if (ret_val == -1) return -1;

    #ifdef HASH_LOGS

        ret_val = hash_table_dump(&hash_table, logs_file);
        if (ret_val == -1) return -1; 

    #endif

    ret_val = hash_table_dtor(&hash_table);
    if (ret_val == -1) return -1;

    return 0;
}

//-----------------------------------------------

static int _hash_table_flush_stats(Hash_table* hash_table, FILE* out FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HASH_TABLE_VALID(hash_table);
    FILE_PTR_CHECK(out);

    for (unsigned int counter = 0;
                      counter < hash_table->capacity;
                      counter ++)
    {
        fprintf(out, "%d", (hash_table->data[counter]).size);

        if (counter != hash_table->capacity - 1)
            fprintf(out, ", ");
        else 
            fprintf(out, "\n");
    }

    return 0;
}

//-----------------------------------------------

int _hash_table_stress_test(const char* src, uint32_t (*hash_func) (void*, unsigned int) 
                                                                 FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    FILENAME_CHECK(src);

    if (!hash_func)
    {
        error_report(INV_HASH_FUNC_PTR);
        return -1;
    }

    Hash_table hash_table = { 0 };
    int ret_val = hash_table_ctor(&hash_table);
    if (ret_val == -1) return -1;
 
    ret_val = hash_table_set_hash_func(&hash_table, hash_func);
    if (ret_val == -1) return -1;

    Hamlet hamlet = { 0 };
    ret_val = hamlet_init(&hamlet, src);
    if (ret_val == -1) return -1;

    ret_val = hash_table_stress_test_perf(&hamlet, &hash_table);
    if (ret_val == -1) return -1;

    ret_val = hash_table_dtor(&hash_table);
    if (ret_val == -1) return -1;

    ret_val = hamlet_destruct(&hamlet);
    if (ret_val == -1) return -1;

    return 0;
 }

 //----------------------------------------------

 static int _hash_table_stress_test_perf(const Hamlet* hamlet, Hash_table* hash_table 
                                                              FOR_LOGS(, LOG_PARAMS))
{
    hash_log_report();
    HASH_TABLE_PTR_CHECK(hash_table);
    HAMLET_PTR_CHECK(hamlet);

    int ret_val = 0;
    List* list  = 0;
    int is_err  = 0;

    clock_t start_time = clock();

    // Insert

    for (unsigned long long counter = 0;
                            counter < hamlet->number;
                            counter++)
    {
        //printf("number %lld", counter);
        //fflush(stdout);
        ret_val = hash_table_smart_insert(hash_table, hamlet->tokens[counter].data, 
                                                      hamlet->tokens[counter].len, 
                                                      &list);

        if (ret_val == -1)
            return -1;
    }

    // clock_t gap_start = clock();

    // #ifdef HASH_LOGS

    //     ret_val = hash_table_dump(hash_table, logs_file);
    //     if (ret_val == -1) return -1;

    // #endif 

    // clock_t gap = clock() - gap_start;

    // Search and delete

    for (unsigned long long counter = 0;
                            counter < hamlet->number;
                            counter++)
    {
        for (unsigned int search_ct = 0;
                          search_ct < 128;
                          search_ct++)
        {
            ret_val = hash_table_search(hash_table, hamlet->tokens[counter].data, 
                                                    hamlet->tokens[counter].len, 
                                                    &list);

            if (ret_val == -1)
                return -1;
            
        }
        
        if (ret_val == ELEMENT_NOT_FOUND)
            continue;

        ret_val = hash_table_delete(hash_table, ret_val, list);
        if (ret_val == -1) return -1;
    }

    clock_t overall_time = clock() - start_time;

    #ifdef HASH_LOGS

        ret_val = hash_table_dump(hash_table, logs_file);
        if (ret_val == -1) return -1;

    #endif 

    if (hash_table->size != 0)
    {
        printf("\n Size: %u Size of hash_table after deleting all elems is not zero \n", 
                                                                      hash_table->size);
        return -1;
    }

    printf("\n TOTAL TIME: %lf \n", (double)(overall_time) / CLOCKS_PER_SEC);

    return 0;
}
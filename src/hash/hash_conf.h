#pragma once 

//===============================================

#include "../global_conf.h"
#include "../list/list_config.h"

//===============================================

#ifdef LOGS 

    #define HASH_LOGS

#endif 

//===============================================

//Configuring definitions for hash table 

//Option enables resizing hash table, if fill factor is too big
//#define HASH_TABLE_RESIZE

static const float Resize_fill_factor = 0.6f;

#ifdef HASH_LOGS

    //Calls dump every time hash table being checked with validator
    //#define HASH_TABLE_DUMP

    #define HASH_TABLE_LIST_DUMPS

#endif 

#define HASH_TABLE_VALID_CHECK

//
#define HASH_ONLY_DUMPS
//

//===============================================

//Hash table initial parameters

static const unsigned int Hash_table_init_cap = 8;

static const unsigned int Hash_table_max_cap  = 256;

//===============================================

static const unsigned char Hash_table_poison_value = 228; 

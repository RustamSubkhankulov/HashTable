#pragma once 

//===============================================

#include "../global_conf.h"
#include "../list/list_config.h"

//===============================================

#ifdef LOGS 

    // Turns on log report every time hash_table
    // related function is called
    //#define HASH_LOGS

#endif 

//===============================================

//Configuring definitions for hash table 

//Option enables resizing hash table, if fill factor is too big
//#define HASH_TABLE_RESIZE

static const float Resize_fill_factor = 0.6f;

//-----------------------------------------------

#ifdef HASH_LOGS

    //Calls dump every time hash table being checked with validator
    //#define HASH_TABLE_DUMP

    // Calls dump for every list in hash table every time
    // hash_table_dump is called
    #define HASH_TABLE_LIST_DUMPS

#endif 

//-----------------------------------------------

// Turns on hash table validator 
//#define HASH_TABLE_VALID_CHECK

#ifdef HASH_TABLE_VALID_CHECK

    // Calls validator for every lsit in hash table
    #define HASH_TABLE_LIST_VALID_CHECK

#endif

//-----------------------------------------------

// If is not defined, turns off all log report 
// remaining only dumps for structure
#define HASH_ONLY_DUMPS

//===============================================

//Hash table initial parameters

static const unsigned int Hash_table_init_cap = 4096;

static const unsigned int Hash_table_max_cap  = 4096;

//===============================================

static const unsigned char Hash_table_poison_value = 228; 

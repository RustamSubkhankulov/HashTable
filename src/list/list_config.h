#pragma once

#include <cstddef>

//===================================================================

#include "../global_conf.h"

#ifdef LOGS

    #define LIST_LOGS

#endif 

//===================================================================

#define SYSTEM_COMMAND_BUF_SIZE 200

//===================================================================

//LIST PARAMETERS

//Element type of the list structure
typedef int elem_t;

//Name of type using in list(for logs)
#define TYPE_NAME "int"

//Specificator for type using in list
#define ELEM_SPEC "%d"

//Starting list capacity
const size_t List_start_capacity = 4;

//Max list capacity
const size_t List_max_capacity = 16;

//===================================================================

//LIST OPTIONS

//Calls dump of the list every time validator is called
#define LIST_DEBUG

//Hash protection for list
#define LIST_HASH

//GRAPHVIZ
#define LIST_GRAPHVIZ

//==================================================================

//SUPPORTING DEFINITIONS

#define POISON_VALUE 228

//==================================================================

//GENERAL

//Turns on logs in general use functions
#define GENERAL_LOGS

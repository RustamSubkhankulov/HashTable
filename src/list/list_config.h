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
typedef char* elem_t;

//Name of type using in list(for logs)
#define TYPE_NAME "char*"

//Specificator for type using in list
#define ELEM_SPEC "%s"

//Starting list capacity
const size_t List_start_capacity = 24;

//Max list capacity
const size_t List_max_capacity = 48;

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

#define LIST_POISON_VALUE 228

//==================================================================

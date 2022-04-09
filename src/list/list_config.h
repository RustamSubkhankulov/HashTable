#pragma once

#include <cstddef>
#include "../global_conf.h"

//===================================================================

#include "../global_conf.h"

#ifdef LOGS

    #define LIST_LOGS

#endif 

//===================================================================

#define SYSTEM_COMMAND_BUF_SIZE 200

//===================================================================

//LIST PARAMETERS

//Starting list capacity
const size_t List_start_capacity = 4;

//Max list capacity
const size_t List_max_capacity = 128;

//===================================================================

//LIST OPTIONS

//Calls dump of the list every time validator is called
//#define LIST_DEBUG

//Hash protection for list
//#define LIST_HASH

//GRAPHVIZ
//#define LIST_GRAPHVIZ

//#define LIST_PTR_CNTRL

//==================================================================

//SUPPORTING DEFINITIONS

#define LIST_POISON_VALUE 228

//==================================================================

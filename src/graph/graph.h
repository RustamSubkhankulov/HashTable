#pragma once 

#include "graph_conf.h"
#include "../log.hs/errors_and_logs"

//===============================================

#ifdef GRAPH_LOGS

    #define graph_log_report() \
            log_report()

#else 

    #define graph_log_repotr() ""

#endif 

//===============================================

#include "global_conf.h"
#include "hash/hash.h"
#include "list/list.h"

//===============================================

int main(int argc, char* argv[])
{
    #ifdef LOGS

        FILE* logs_file = open_log_file(Logfile_name);
        if (!logs_file)
            return -1;

    #endif 

    Hamlet hamlet = { 0 };

    int ret_val = hamlet_init(&hamlet, "src/hamlet/draft.txt");
    if (ret_val == -1)
        return -1;

    ret_val = hamlet_print_data(&hamlet);
    if (ret_val == -1)
        return -1;

    ret_val = hamlet_destruct(&hamlet);
    if (ret_val == -1)
        return -1;

    //Hash_table hash_table = { 0 };

    // int ret_val = hash_table_testing(&hash_table, &my_hash);
    // if (ret_val == -1)
    //     return -1;

    #ifdef LOGS
        close_log_file();
    #endif 

    return 0;
}
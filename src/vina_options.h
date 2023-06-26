#ifndef _OPTIONS_
#define _OPTIONS_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "arguments.h"
#include "vina_aux_functions.h"
#include "archiver.h"

typedef struct
{
        char *name;
        uid_t user_id;
        mode_t permissions;
        unsigned long int size;
        time_t modification_date;
} member_data_t;

int option_insert(arguments_t *arg);
int option_insert_a(arguments_t *arg);
int option_move(arguments_t *arg);
int option_extract(arguments_t *arg);
int option_remove(arguments_t *arg);
int option_list(arguments_t *arg);
int option_help(arguments_t *arg);

#endif
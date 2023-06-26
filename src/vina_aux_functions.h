#ifndef _VINA_AUX_FUNCTIONS_
#define _INVA_AUX_FUNCTIONS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "arguments.h"
#include "archiver.h"

typedef struct
{
        char *name;
        uid_t user_id;
        mode_t permissions;
        unsigned long int size;
        time_t modification_date;
} member_data_t;

typedef struct
{
        unsigned int directory_size;
        int file_count;
        member_data_t **members;
} archive_data_t;

void initialize_archive(FILE *archive);

archive_data_t *get_archive_data(FILE *archive);

#endif
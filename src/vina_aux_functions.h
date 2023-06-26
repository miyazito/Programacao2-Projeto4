#ifndef _VINA_AUX_FUNCTIONS_
#define _INVA_AUX_FUNCTIONS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
// #include "arguments.h"
#include "archiver.h"

typedef struct
{
        unsigned int name_size;
        char *name;
        uid_t user_id;
        mode_t permissions;
        off_t size;
        time_t modification_date;
        unsigned int archive_order;
        unsigned long int position;
} member_data_t;

typedef struct
{
        unsigned int directory_size;
        int file_count;
        member_data_t **members;
} archive_data_t;

void initialize_archive(FILE *archive);

archive_data_t *get_archive_data(FILE *archive);

member_data_t *get_member_data(FILE *member, char *member_name, unsigned int archive_order, unsigned int position);

#endif
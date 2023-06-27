#ifndef _VINA_AUX_FUNCTIONS_
#define _INVA_AUX_FUNCTIONS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include "archiver.h"

typedef struct
{
        unsigned int member_data_size;
        unsigned int name_size;
        char *name;
        uid_t user_id;
        mode_t permissions;
        off_t size;
        time_t modification_date;
        unsigned int archive_order;
        unsigned int position;
} member_data_t;

typedef struct
{
        unsigned int directory_position, directory_size;
        int file_count;
        member_data_t **members;
} archive_data_t;

void initialize_archive(FILE *archive);

archive_data_t *get_archive_data(char *archive_name, FILE *archive);

member_data_t *get_member_data(FILE *member, char *member_name, unsigned int archive_order, unsigned int position);

member_data_t *get_member_data_from_archive(FILE *archive);

void put_member_data(member_data_t *member_data, FILE *archive);

void destroy_archive_data(archive_data_t *archive_data);

int file_is_in_archive(char *filename, archive_data_t *archive_data);

void extract_file(FILE *archive, FILE *file, unsigned int total_bytes, unsigned int position);

void update_directory(FILE *archive, archive_data_t *archive_data, unsigned int old_position, unsigned int new_position, unsigned int old_size, unsigned int new_size, int new_file_count);

#endif
#include "vina_aux_functions.h"

void initialize_archive(FILE *archive)
{
        unsigned int directory_size = sizeof(unsigned int) + sizeof(int);
        int file_count = 0;

        fseek(archive, 0, SEEK_SET);
        fwrite(&directory_size, sizeof(unsigned int), 1, archive);
        fwrite(&file_count, sizeof(int), 1, archive);
}

archive_data_t *get_archive_data(FILE *archive)
{
        archive_data_t *archive_data = (archive_data_t *)malloc(sizeof(archive_data_t));

        fseek(archive, 0, SEEK_SET);
        fread(&archive_data->directory_size, sizeof(unsigned int), 1, archive);
        fread(&archive_data->file_count, sizeof(int), 1, archive);

        return archive_data;
}
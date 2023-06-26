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

member_data_t *get_member_data(FILE *member, char *member_name, unsigned int archive_order, unsigned int position)
{
        struct stat file_info; 
        member_data_t *member_data = (member_data_t *)malloc(sizeof(member_data_t));

        if (stat(member_name, &file_info) == -1)
        {
                fprintf(stderr, "Erro ao obter informações do arquivo %s\n", member_name);
                free(member_data);
                return NULL;
        }

        member_data->name_size = strlen(member_name);
        member_data->name = member_name;
        member_data->user_id = file_info.st_uid;
        member_data->permissions = file_info.st_mode;
        member_data->size = file_info.st_size;
        member_data->modification_date = file_info.st_mtime;
        member_data->archive_order = archive_order;
        member_data->position = position;

        return member_data;
}
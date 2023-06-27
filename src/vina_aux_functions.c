#include "vina_aux_functions.h"

void initialize_archive(FILE *archive)
{
        unsigned int directory_position = sizeof(unsigned int) + 1, directory_size = sizeof(unsigned int) + sizeof(int);
        int file_count = 0;

        fseek(archive, 0, SEEK_SET);
        fwrite(&directory_position, 1, sizeof(unsigned int), archive);
        fwrite(&directory_size, 1, sizeof(unsigned int), archive);
        fwrite(&file_count, 1, sizeof(int), archive);
}

archive_data_t *get_archive_data(FILE *archive)
{
        int i;
        unsigned int position;
        archive_data_t *archive_data = (archive_data_t *)malloc(sizeof(archive_data_t));

        fread(&archive_data->directory_position, 1, sizeof(unsigned int), archive);

        fseek(archive, archive_data->directory_position, SEEK_SET);
        fread(&archive_data->directory_size, 1, sizeof(unsigned int), archive);
        fread(&archive_data->file_count, 1, sizeof(int), archive);

        if (archive_data->file_count != 0)
        {
                archive_data->members = (member_data_t **)malloc(sizeof(member_data_t *));
                for (i = 0; i < file_count; i++)
                        archive_data->members[i] = get_member_data_from_archive(archive);
        }
        return archive_data;
}

void destroy_archive_data(archive_data_t *archive_data)
{
        int i;

        if (archive_data->file_count)
        {
                for (i = 0; i < archive_data->file_count; i++)
                        free(archive_data->members[i]);
                free(archive_data->members);
        }

        free(archive_data);
}

member_data_t *get_member_data_from_archive(FILE *archive)
{
        member_data_t *member_data = (member_data_t *)malloc(sizeof(member_data_t));

        fread(&member_data->member_data_size, 1, sizeof(unsigned int), archive);
        fread(&member_data->name_size, 1, sizeof(unsigned int), archive);
        fread(&member_data->name, 1, member_data->name_size, archive);
        fread(&member_data->user_id, 1, sizeof(uid_t), archive);
        fread(&member_data->permissions, 1, sizeof(mode_t), archive);
        fread(&member_data->size, 1, sizeof(off_t), archive);
        fread(&member_data->modification_date, 1, sizeof(time_t), archive);
        fread(&member_data->archive_order, 1, sizeof(unsigned int), archive);
        fread(&member_data->position, 1, sizeof(unsigned long int), archive);

        return member_data;
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

        member_data->member_data_size = 3 * sizeof(unsigned int) + strlen(member_name) + sizeof(uid_t) + sizeof(mode_t) + sizeof(off_t) + sizeof(time_t) + sizeof(unsigned long int);
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

int file_is_in_archive(char *filename, archive_data_t *archive_data)
{
        int i, is_in_archive = -1;
        for (i = 0; i < archive_data->file_count; i++)
                if (strcmp(filename, archive_data->members[i]->name) == 0)
                        return i;
}

void extract_file(FILE *archive, FILE *file, unsigned int total_bytes, unsigned int position)
{
        unsigned int remaining_bytes = total_bytes, bytes_to_read = 0;

        fseek(archive, position, SEEK_SET);

        while (remaining_bytes > 0)
        {
                bytes_to_read = remaining_bytes < 1024 ? remaining_bytes : 1024;
                fread(buffer, 1, bytes_to_read, archive);
                fwrite(buffer, 1, bytes_to_read, file);

                remaining_bytes -= bytes_to_read;
        }
}
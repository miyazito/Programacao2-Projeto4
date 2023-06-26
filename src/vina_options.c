#include "vina_options.h"

int option_insert(arguments_t *arg)
{
        int i;
        unsigned int member_size, position, archive_order, member_directory_data_size, old_directory_size;
        char *buffer;
        FILE *archive, *members[arg->file_count];
        archive_data_t *archive_data;
        member_data_t * member_data;

        if (!(archive = fopen(arg->archive, "rb+")))
        {
                fprintf(stderr, "Erro ao abrir o arquivo \"%s\"\n", arg->archive);
                return 0;
        }
        
        if(!get_size(archive))
                initialize_archive(archive);

        archive_data = get_archive_data(archive);
        archive_order = archive_data->file_count + 1;

        for (i = 0; i < arg->file_count; i++)
        {
                buffer = (char *)malloc(sizeof(char) * 1024);
                member_directory_data_size = strlen(arg->files[i]) + sizeof(uid_t) + sizeof(mode_t) + sizeof(off_t) + sizeof(time_t) + sizeof(unsigned int) + sizeof(unsigned long int);
                if (!(members[i] = fopen(arg->files[i], "rb")))
                {
                        fprintf(stderr, "Erro ao abrir arquivo \"%s\"\n", arg->files[i]);
                        return 0;
                }
                member_size = get_size(members[i]);
                
                old_directory_size = archive_data->directory_size;
                archive_data->directory_size += member_directory_data_size;

                position = archive_data->directory_size + 1;
                member_data = get_member_data(members[i], arg->files[i], archive_order, position);

                if (archive_data->file_count != 0)
                        move_bytes(archive, old_directory_size + 1, get_size(archive), archive_data->directory_size + 1);

                fseek(archive, old_directory_size + 1, SEEK_SET);
                fwrite(&member_data->name_size, sizeof(unsigned int), 1, archive);
                fwrite(&member_data->name, member_data->name_size + 1, 1, archive);
                fwrite(&member_data->user_id, sizeof(uid_t), 1, archive);
                fwrite(&member_data->permissions, sizeof(mode_t), 1, archive);
                fwrite(&member_data->size, sizeof(off_t), 1, archive);
                fwrite(&member_data->modification_date, sizeof(time_t), 1, archive);
                fwrite(&member_data->archive_order, sizeof(unsigned int), 1, archive);
                fwrite(&member_data->position, sizeof(unsigned long int), 1, archive);
                
                // printf("member name: %s\n", member_data->name);
                // printf("member user id: %u\n", member_data->user_id);
                // printf("member permissions: %o\n", member_data->permissions);
                // printf("member size: %jd\n", member_data->size);
                // printf("member modification date: %s\n", ctime(&member_data->modification_date));
                // printf("member archive order: %d\n", member_data->archive_order);
                // printf("member position: %lu\n", member_data->position);

                
                archive_order++;


                extract_bytes(members[i], buffer, 1, member_size);
                append_bytes(archive, buffer, member_size);

                fclose(members[i]);
                free(buffer);
        }
        fclose(archive);

        return 1;
}

int option_insert_a(arguments_t *arg)
{
        FILE *archive = fopen(arg->archive, "rb+");
        archive_data_t *archive_data = get_archive_data(archive);

        printf("directory size: %u\nfile count: %d\n", archive_data->directory_size, archive_data->file_count);
        
        free(archive_data);
        fclose(archive);
        return 1;
}

int option_move(arguments_t *arg)
{
        
        return 1;
}

int option_extract(arguments_t *arg)
{
        unsigned int member_size;
        char *buffer = (char *)malloc(sizeof(char) * 10000);
        FILE *archive, *member;

        if (!(archive = fopen(arg->archive, "rb+")))
        {
                fprintf(stderr, "Erro ao abrir o arquivo \"%s\"\n", arg->archive);
                return 0;
        }

        if (!(member = fopen(arg->files[0], "wb+")))
        {
                fprintf(stderr, "Erro ao abrir/criar arquivo \"%s\"\n", arg->files[0]);
                return 0;
        }
        member_size = get_size(archive);
        extract_bytes(archive, buffer, 1, member_size);
        append_bytes(member, buffer, member_size);

        free(buffer);
        fclose(member);
        fclose(archive);

        return 1;
}

int option_remove(arguments_t *arg)
{
        FILE *archive = fopen(arg->archive, "rb+");
        archive_data_t *archive_data;
        member_data_t *member_data;
        fopen(arg->archive, "rb+");

        fseek(archive, 0, SEEK_SET);

        archive_data = (archive_data_t *)malloc(sizeof(archive_data_t));
        member_data = (member_data_t *)malloc(sizeof(member_data_t));

        fread(&archive_data->directory_size, sizeof(unsigned int), 1, archive);
        fread(&archive_data->file_count, sizeof(int), 1, archive);
        fread(&member_data->name_size, sizeof(unsigned int), 1, archive);
        fread(&member_data->name, member_data->name_size, 1, archive);
        fread(&member_data->user_id, sizeof(uid_t), 1, archive);
        fread(&member_data->permissions, sizeof(mode_t), 1, archive);
        fread(&member_data->size, sizeof(off_t), 1, archive);
        fread(&member_data->modification_date, sizeof(time_t), 1, archive);
        fread(&member_data->archive_order, sizeof(unsigned int), 1, archive);
        fread(&member_data->position, sizeof(unsigned long int), 1, archive);
        
        

        printf("archive directory size: %d\n", archive_data->directory_size);
        printf("archive file_count: %d\n", archive_data->file_count);
        printf("member name size: %d\n", member_data->name_size);
        printf("member name: %s\n", member_data->name);
        printf("member user id: %u\n", member_data->user_id);
        printf("member permissions: %o\n", member_data->permissions);
        printf("member size: %jd\n", member_data->size);
        printf("member modification date: %s\n", ctime(&member_data->modification_date));
        printf("member archive order: %d\n", member_data->archive_order);
        printf("member position: %lu\n", member_data->position);


        fclose(archive);
        free(archive_data);
        free(member_data);

        return 1;
}

int option_list(arguments_t *arg)
{
        return 1;
}

int option_help(arguments_t *arg)
{
        printf("Uso: vina++ <opcao> <archive> [membro1 membro2 ...]\n");
        printf("Opcoes:\n");
        printf("-i : insere um ou mais membros ao archive\n");
        printf("-a : mesmo comportamento da opcao -i, mas substitui apenas se o parametro for mais recente\n");
        printf("-m target : move o membro indicado para imediatamente apos o membro target\n");
        printf("-x : extrai os membros indicados de archive\n");
        printf("-r : remove os membros indicados de archive\n");
        printf("-c : lista o conteudo de archive\n");
        printf("-h : imprime esta mensagem de ajuda\n");

        return 1;
}
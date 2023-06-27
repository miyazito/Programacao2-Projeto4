#include "vina_options.h"

int option_insert_a(arguments_t *arg)
{
        int i;
        unsigned int member_size, position, archive_order, old_directory_size/*, directory_position*/;
        char *buffer;
        FILE *archive, *members[arg->member_count];
        archive_data_t *archive_data;
        member_data_t *member_data;

        if (!(archive = fopen(arg->archive, "rb+")))
        {
                fprintf(stderr, "Erro ao abrir o arquivo \"%s\"\n", arg->archive);
                return 0;
        }

        if (!get_size(archive))
                initialize_archive(archive);

        archive = freopen(arg->archive, "rb+", archive);

        archive_data = get_archive_data(archive);

        for (i = 0; i < arg->member_count; i++)
        {
                buffer = (char *)malloc(sizeof(char) * 1024);

                if (!(members[i] = fopen(arg->files[i], "rb")))
                {
                        fprintf(stderr, "Erro ao abrir arquivo \"%s\"\n", arg->files[i]);
                        return 0;
                }
                member_size = get_size(members[i]);

                

                position = archive_data->directory_position;
                member_data = get_member_data(members[i], arg->files[i], archive_order, position);
                old_directory_size = archive_data->directory_size;
                archive_data->directory_size += member_data->member_data_size;
                move_bytes(archive, position, position + old_directory_size, position + member_size);

                fseek(archive, position, SEEK_SET);
                fread(buffer, 1, member_size, members[i]);
                fwrite(buffer, 1, member_size, archive);

                put_member_data(member_data, archive);                

                archive_order++;

                fclose(members[i]);
                free(buffer);
        }
        fclose(archive);

        return 1;
}

int option_insert(arguments_t *arg)
{
        return 1;
}

int option_move(arguments_t *arg)
{

        return 1;
}

int option_extract(arguments_t *arg)
{
        int i, file_index;
        FILE *archive, *member;
        archive_data_t *archive_data;

        archive = fopen(arg->archive, "rb+");
        archive_data = get_archive_data(archive);

        if (arg->member_count == 0)
        {
                for (i = 0; i < archive_data->file_count; i++)
                {
                        member = fopen(archive_data->members[i]->name, "wb+");
                        extract_file(archive, member, archive_data->members[i]->size, archive_data->members[i]->position);
                        fclose(member);
                }
        }
        else
        {
                for (i = 0; i < arg->member_count; i++)
                {
                        if ((file_index = file_is_in_archive(arg->files[i], archive_data)) != -1)
                        {
                                member = fopen(archive_data->members[file_index]->name, "wb+");
                                extract_file(archive, member, archive_data->members[file_index]->size, archive_data->members[file_index]->position);
                                fclose(member);
                        }
                        else
                                fprintf(stderr, "Membro %s nao esta no arquivador\n", arg->files[i]);
                }
        }

        destroy_archive_data(archive_data);
        fclose(archive);

        return 1;
}

int option_remove(arguments_t *arg)
{/*
        FILE *archive;
        archive_data_t *archive_data;

        archive = fopen(arg->archive, "rb+");
        archive_data = get_archive_data(archive);*/

        return 1;
}

int option_list(arguments_t *arg)
{
        int i;
        FILE *archive;
        archive_data_t *archive_data;

        if (!(archive = fopen(arg->archive, "rb+")))
        {
                fprintf(stderr, "Erro ao abrir o arquivador %s\n", arg->archive);
                return 0;
        }

        archive_data = get_archive_data(archive);

        if (archive_data->file_count == 0)
                printf("O arquivador %s esta vazio\n", arg->archive);
        else
        {
                printf("Arquivos presentes no arquivador %s:\n", arg->archive);
                for (i = 0; i < archive_data->file_count; i++)
                        printf("%s\n", archive_data->members[i]->name);
        }

        destroy_archive_data(archive_data);
        fclose(archive);

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

int option_test(arguments_t *arg)
{
        
        return 1;
}
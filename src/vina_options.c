#include "vina_options.h"

int option_insert(arguments_t *arg)
{
        int i;
        unsigned int member_size, position = 1;
        char *buffer;
        FILE *archive, *members[arg->file_count];

        if (!(archive = fopen(arg->archive, "rb+")))
        {
                fprintf(stderr, "Erro ao abrir o arquivo \"%s\"\n", arg->archive);
                return 0;
        }
        /*
                if (feof(archive))
                        if (!initialize_archive(archive))
                                return 0;
        */
        for (i = 0; i < arg->file_count; i++)
        {
                buffer = (char *)malloc(sizeof(char) * 10000);
                if (!(members[i] = fopen(arg->files[i], "rb")))
                {
                        fprintf(stderr, "Erro ao abrir arquivo \"%s\"\n", arg->files[i]);
                        return 0;
                }
                member_size = get_size(members[i]);
                extract_bytes(members[i], buffer, position, member_size);
                append_bytes(archive, buffer, member_size);

                fclose(members[i]);
                free(buffer);
        }
        fclose(archive);

        return 1;
}

int option_insert_a(arguments_t *arg)
{
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
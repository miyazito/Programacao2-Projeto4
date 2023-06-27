#include "arguments.h"

arguments_t *create_arguments()
{
	arguments_t *arg = (arguments_t *)malloc(sizeof(arguments_t));

	arg->option = 0;
	arg->archive = NULL;
	arg->files = NULL;
	arg->target = NULL;
	arg->member_count = 0;

	return arg;
}

void destroy_arguments(arguments_t *arg)
{
	int i;

	if (arg->archive)
		free(arg->archive);
	if (arg->files)
	{
		while (arg->member_count != 0)
		{
			i = arg->member_count - 1;
			free(arg->files[i]);
			arg->member_count--;
		}
		free(arg->files);
	}
	free(arg);
}

void error(arguments_t *arg)
{
	fprintf(stderr, "Execucao errada do programa.\nRode \"vina++ -h\" para exemplo de execucao correta.\n");
	destroy_arguments(arg);
}

int treat_arguments(int argc, char **argv, arguments_t *arg)
{
	int move_flag = 0, i;

	if (argc < 3 || (arg->option == MOVE && argc < 5))
		return 0;

	if (arg->option == MOVE)
	{
		move_flag = 1;
		arg->target = (char *)malloc(sizeof(char) * (strlen(argv[2]) + 1));
		strcpy(arg->target, argv[2]);
	}
	arg->archive = (char *)malloc(sizeof(char) * (strlen(argv[2 + move_flag]) + 1));
	strcpy(arg->archive, argv[2 + move_flag]);
	arg->files = (char **)malloc(sizeof(char *));
	for (i = 3 + move_flag; i < argc; i++)
	{
		arg->files = (char **)realloc(arg->files, sizeof(char *) * (arg->member_count + 1));
		arg->files[i - (3 + move_flag)] = (char *)malloc(sizeof(char) * (strlen(argv[i + move_flag]) + 1));
		strcpy(arg->files[i - (3 + move_flag)], argv[i + move_flag]);
		arg->member_count++;
	}

	return 1;
}

arguments_t *check_arguments(int argc, char **argv)
{
	arguments_t *arg = create_arguments();
	char option;
	int option_selected = 0;

	if (argc < 2)
	{
		error(arg);
		return NULL;
	}

	while ((option = getopt(argc, argv, "iamxrcht")) != -1)
	{
		if (option_selected)
		{
			error(arg);
			return NULL;
		}

		switch (option)
		{
		case 'i':
			option_selected = 1;
			arg->option = INSERT;
			break;
		case 'a':
			option_selected = 1;
			arg->option = INSERT_A;
			break;
		case 'm':
			option_selected = 1;
			arg->option = MOVE;
			break;
		case 'x':
			option_selected = 1;
			arg->option = EXTRACT;
			break;
		case 'r':
			option_selected = 1;
			arg->option = REMOVE;
			break;
		case 'c':
			option_selected = 1;
			arg->option = LIST;
			break;
		case 'h':
			option_selected = 1;
			arg->option = HELP;
			break;
		case 't':
			option_selected = 1;
			arg->option = TEST;
			break;
		default:
			error(arg);
			return NULL;
			break;
		}
	}

	if (arg->option != HELP)
		if (!treat_arguments(argc, argv, arg))
		{
			error(arg);
			return NULL;
		}

	return arg;
}
/*
void test_arguments(arguments_t *arg)
{
	int i;
	if (arg->option != HELP)
	{
		printf("option: %d\n", arg->option);
		printf("archive: %s\n", arg->archive);
		printf("member count: %d\n", arg->member_count);
		printf("files:");
		for (i = 0; i < arg->member_count; i++)
			printf(" %s", arg->files[i]);
		printf("\n");
	}
	else
		printf("help\n");
}*/
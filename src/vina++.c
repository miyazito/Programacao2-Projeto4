#include <stdio.h>
#include <stdlib.h>
#include "arguments.h"
#include "vina_options.h"
#include "archiver.h"

int main(int argc, char **argv)
{
	arguments_t *arg;
	int (*option_function)(arguments_t *);

	if (!(arg = check_arguments(argc, argv)))
		return 1;

	// test_arguments(arg);

	switch (arg->option)
	{
	case INSERT:
		option_function = option_insert;
		break;
	case INSERT_A:
		option_function = option_insert_a;
		break;
	case MOVE:
		option_function = option_move;
		break;
	case EXTRACT:
		option_function = option_extract;
		break;
	case REMOVE:
		option_function = option_remove;
		break;
	case LIST:
		option_function = option_list;
		break;
	case HELP:
		option_function = option_help;
		break;
	case TEST:
		option_function = option_test;
		break;
	}

	if (!option_function(arg))
	{
		destroy_arguments(arg);
		return 2;
	}

	destroy_arguments(arg);

	return 0;
}
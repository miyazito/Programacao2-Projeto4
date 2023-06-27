#ifndef _ARGUMENTS_
#define _ARGUMENTS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

typedef enum
{
	INSERT,
	INSERT_A,
	MOVE,
	EXTRACT,
	REMOVE,
	LIST,
	HELP,
	TEST
} option_t;

typedef struct
{
	option_t option;
	char *archive, **files, *target;
	int member_count;
} arguments_t;

arguments_t *check_arguments(int argc, char **argv);

void test_arguments(arguments_t *arg);

void destroy_arguments(arguments_t *arg);

#endif
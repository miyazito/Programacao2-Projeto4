#include "vina_aux_functions.h"

int initialize_archive(FILE *archive)
{
        unsigned int directory_size = sizeof(unsigned int) + sizeof(int);
        int file_count = 0;

        fwrite(&directory_size, 1, (int)sizeof(unsigned int), archive);
        fwrite(&file_count, 1, (int)sizeof(int), archive);

        return 1;
}
#include "utils.h"

long total_file_size(char *file_name)
{
	FILE *fp_read;
	long start_position, end_position;

	if (!(fp_read = fopen(file_name, "rb"))) 
	{
		fprintf(stderr, "Error opening binary file\n");
		exit(1);
    }

    start_position = ftell(fp_read);
    fseek(fp_read, 0, SEEK_END);
    end_position = ftell(fp_read);
    fclose(fp_read);

    return end_position - start_position;
}

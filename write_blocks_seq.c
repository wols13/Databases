#include <stdio.h>
#include <stdlib.h>
#include "write_blocks_seq.h"



void csv_to_records(const char * csv_path, long block_size){
	FILE * fp;
    char * line = NULL;
    size_t len;
    int lines_read = 0;
    
    int records_per_block = block_size / sizeof(Record);
    Record * records = malloc(records_per_block * sizeof(Record));

    fp = fopen(csv_path, "r");
    if (fp == NULL) {
        fprintf(stderr,"Error opening file\n");
        exit(1);
    }

    while (getline(&line, &len, fp) != -1) {
        records[lines_read].uid1 = strtok(line, ' ');
        records[lines_read].uid2 = strtok(NULL, ' ');
        
        lines_read++;
        if (lines_read >= records_per_block) {
			/* appends the full page of serialized records to a binary file */
		}
    }

    fclose(fp);
    if (line)
        free(line);
}

int main( int argc, const char* argv[] )
{
	if (argc != 3){
        fprintf(stderr,"Proper format: write_blocks_seq <input filename> <block size> \n");
        exit(1);
	}
	
	csv_to_records(argv[1], strtol(argv[2], NULL, 10));
}

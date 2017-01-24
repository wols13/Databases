#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include "write_blocks_seq.h"



/*
 * Now it is time to implement a script in a scripting language of your choice (Python, bash),
 * which runs write_blocks_seq with different block size parameters, and outputs the processing
 * rate in Bytes Per Second (BPS) to stdout (can be redirected to a file). */

void csv_to_records(const char * csv_path, long block_size){
	FILE *fp, *fp_write;
    char line[MAX_CHARS_PER_LINE];
    size_t len;
    int lines_read = 0;
    struct timeb t_begin, t_end;
    long time_spent_ms;
    
    int records_per_block = block_size / sizeof(Record);
    Record * records = malloc(records_per_block * sizeof(Record));

    if (!(fp = fopen(csv_path, "r"))) {
        fprintf(stderr,"Error opening csv file\n");
        exit(1);
    }

	if (!(fp_write = fopen("records.dat", "wb"))) {
		fprintf(stderr,"Error opening .dat file\n");
		exit(1);
    }

	/* Start recording total write time */
	ftime(&t_begin);

    while (fgets(line, MAX_CHARS_PER_LINE, fp) != NULL) {
		line[strcspn(line, "\r\n")] = '\0';

		/* Converting each csv line to struct Record instances */
        records[lines_read].uid1 = atoi(strtok(line, " "));
        records[lines_read].uid2 = atoi(strtok(NULL, " "));
        
        lines_read++;
        if (lines_read >= records_per_block) {
			lines_read--;
			break;
		}
    }

	/* Write full block buffer to binary file & flush to disk */
	fwrite(records, sizeof(Record), lines_read, fp_write);
	fflush(fp_write);

	fclose(fp_write);
    fclose(fp);
    
    /* Stop recording total write time */
    ftime(&t_end);
	time_spent_ms = (long) (1000 *(t_end.time - t_begin.time) + (t_end.millitm - t_begin.millitm));
	printf ("Data rate: %.3f MBPS\n", ((lines_read * sizeof(Record))/(float)time_spent_ms * 1000)/MB);

    if (records)
        free(records);
}

int main( int argc, const char* argv[] )
{
	if (argc != 3){
        fprintf(stderr,"Proper format: write_blocks_seq <input filename> <block size> \n");
        exit(1);
	}
	
	csv_to_records(argv[1], strtol(argv[2], NULL, 10));
}

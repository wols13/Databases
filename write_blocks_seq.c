#include "utils.h"

void csv_to_records(const char * csv_path, long block_size){
	FILE *fp, *fp_write;
    char line[MAX_CHARS_PER_LINE];
    int lines_read = 0, block_index = 0;
    struct timeb t_begin, t_end;
    long time_spent_ms;
    
    int records_per_block = block_size / sizeof(Record);
    Record * records = malloc(records_per_block * sizeof(Record));

    if (!(fp = fopen(csv_path, "r"))) {
        fprintf(stderr, "Error opening csv file\n");
        exit(1);
    }

	if (!(fp_write = fopen("records.dat", "wb"))) {
		fprintf(stderr, "Error opening .dat file\n");
		exit(1);
    }

	/* Start recording total write time */
	ftime(&t_begin);

    while (fgets(line, MAX_CHARS_PER_LINE, fp) != NULL) {
		line[strcspn(line, "\r\n")] = '\0';

		/* Converting each csv line to struct Record instances */
        records[block_index].uid1 = atoi(strtok(line, " "));
        records[block_index].uid2 = atoi(strtok(NULL, " "));
        
        lines_read++;
        block_index++;
        if (block_index >= records_per_block) {
			fwrite(records, sizeof(Record), block_index, fp_write);
			block_index = 0;
		}
    }
    
    if (block_index > 0)
		fwrite(records, sizeof(Record), block_index, fp_write);

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
        fprintf(stderr, "Proper format: write_blocks_seq <input filename> <block size> \n");
        exit(1);
	}
	
	csv_to_records(argv[1], strtol(argv[2], NULL, 10));

	return 0;
}

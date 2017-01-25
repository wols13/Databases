#include "utils.h"

void read_blocks_seq(char *file_name, int block_size)
{
	int records_per_block = block_size / sizeof(Record);
	FILE *fp_read;
	int total_records = 0, unique_uid1 = 0, max_following = 0, current_following = 0, last_uid1, i;
	struct timeb t_begin, t_end;
	long time_spent_ms;

	/* allocate buffer for 1 block */
	Record * buffer = (Record *) calloc (records_per_block, sizeof(Record)) ;

	ftime(&t_begin);

	if (!(fp_read = fopen(file_name, "rb"))) {
		fprintf(stderr, "Error opening binary file\n");
		exit(1);
    }

	/* read records into buffer */
	int result = fread(buffer, sizeof(Record), records_per_block, fp_read);
	if (result != 0) {
		last_uid1 = buffer[0].uid1;
		unique_uid1 = 1;
	}
	
	while (result != 0){
		total_records = total_records + result;
		
		for (i = 0; i < result; i++){
			if (buffer[i].uid1 != last_uid1){
				last_uid1 = buffer[i].uid1;
				unique_uid1 = unique_uid1 + 1;
				current_following = 1;
			} else {
				current_following = current_following + 1;
				max_following = MAX(current_following, max_following);
			}
		}
		result = fread(buffer, sizeof(Record), records_per_block, fp_read);
	}
	
	printf("\nMax following: %d, Average following: %f\n", max_following, total_records/(float)unique_uid1);

	ftime(&t_end);
	/* time elapsed in milliseconds */
	time_spent_ms = (long) (1000 *(t_end.time - t_begin.time) + (t_end.millitm - t_begin.millitm)); 

	/* result in MB per second */
	printf ("Data rate: %.3f MBPS\n\n", ((total_records * sizeof(Record)) / (float)time_spent_ms * 1000) / MB);

	fclose (fp_read);
	free (buffer);
}


int main(int argc, char *argv[])
{
	if (argc != 3){
        fprintf(stderr,"Proper format: read_blocks_seq <binary filename> <block size> \n");
        exit(1);
	}

	if (strtol(argv[2], NULL, 10) % sizeof(Record) != 0){
        fprintf(stderr,"Argument 2 (block size) has to be a multiple of sizeof(Record)\n");
        exit(1);
	}
	
	read_blocks_seq(argv[1], strtol(argv[2], NULL, 10));

	return 0;
}

#include "utils.h"

void read_ram_seq(char *file_name)
{
	FILE *fp_read;
	int total_records = 0, unique_uid1 = 0, max_following = 0, current_following = 0, last_uid1, i;
	struct timeb t_begin, t_end;
	long time_spent_ms, binary_file_size;
	
	binary_file_size = total_file_size(file_name);
	long records_in_file = binary_file_size / sizeof(Record);

	/* allocate buffer for entire binary file */
	Record * buffer = (Record *) malloc(binary_file_size);

	if (!(fp_read = fopen(file_name, "rb"))) {
		fprintf(stderr, "Error opening binary file\n");
		exit(1);
    }

	/* read records into buffer */
	int result = fread(buffer, sizeof(Record), records_in_file, fp_read);
	
	if (result != records_in_file){
		fprintf(stderr, "Error reading entire binary file\n");
		exit(1);
	}

	ftime(&t_begin);

	last_uid1 = buffer[0].uid1;
	unique_uid1 = 1;
		
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
	
	printf("\nMax following: %d, Average following: %f\n", max_following, result/(float)unique_uid1);
	
	ftime(&t_end);
	/* time elapsed in milliseconds */
	time_spent_ms = (long) (1000 *(t_end.time - t_begin.time) + (t_end.millitm - t_begin.millitm)); 

	/* result in MB per second */
	printf ("Data rate: %.3f BPS\n\n", (total_records * sizeof(Record)) / (float)time_spent_ms * 1000);

	fclose (fp_read);
	free (buffer);
}


int main(int argc, char *argv[])
{
	if (argc != 2){
        fprintf(stderr,"Proper format: read_ram_seq <binary filename>\n");
        exit(1);
	}
	
	read_ram_seq(argv[1]);

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>

#define KB 1024
#define MB (1024 * 1024)
#define MAX_CHARS_PER_LINE 22
#define MAX(a, b) ((a) > (b) ? a : b)

typedef struct record {
	int uid1;
	int uid2;
} Record; 

long total_file_size(char *file_name);
void csv_to_records(const char * csv_path, long block_size);
void read_blocks_seq(char *file_name, int block_size);
void read_ram_seq(char *file_name);

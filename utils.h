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

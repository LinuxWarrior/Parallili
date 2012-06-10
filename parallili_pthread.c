#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define HOURS 24
#define ROWS 8
#define COLUMNS 8
#define MAX_X 7
#define MAX_Y 7
#define MIN_X 7
#define MIN_Y 7

int curr;
int global_num_of_threads;

struct K
{
	unsigned short i;
	int data[ROWS][COLUMNS];
} katastasi[HOURS];

struct E
{
	unsigned short i;
	int data[ROWS][COLUMNS];
} entasi[HOURS];

struct T
{
	unsigned short i;
	int data[ROWS][COLUMNS];
} therm[HOURS];

void init() {
	int i, j, n;
	FILE *file; 
	
	for (i = 0; i < HOURS; i++) {
		katastasi[i].i = i;
		therm[i].i = i;
		entasi[i].i = i;
	}
	
	for (i = 0; i < HOURS; i++) {
		//printf("%d\n", katastasi[i].i);
		
		for (j = 0; j < ROWS; j++) {
			for (n = 0; n < COLUMNS; n++) {
				katastasi[i].data[j][n] = rand() % 5;
				therm[i].data[j][n] = 22;
				entasi[i].data[j][n] = 0;
				
				if (i == 0) {
					file = fopen("map.000","a+"); 
					fprintf(file,"%d", katastasi[i].data[j][n]);
					fclose(file);
				} 
			}
		}	
	}
	
}

void print_debug() {
	int i,j, n;
	for (i = 0; i < HOURS; i++) {		
		for (j = 0; j < ROWS; j++) {
			for (n = 0; n < COLUMNS; n++)
				printf("%d\n", katastasi[i].data[j][n]);
		}
	}
}

void *decompose_on_x(void *id) {
	int i, j, n, startPosX, startPosY, endPosX, endPosY, rowNum, colNum;
	FILE *file; 
	
	int sum = 0;
	int count = 0;
	int opt = 0;
	
	const char *name = "map.0";
	char buf[24] = { 0 };
	
	int thread_id = *((int*)id);
	
	int from_range;
	int to_range;
	
	if ((thread_id == 0) || (thread_id == global_num_of_threads - 1)) {
		int data[ROWS / global_num_of_threads + 1][COLUMNS / global_num_of_threads];
		/* mutex lock */
		from_range = thread_id * (ROWS / 4);
		to_range = from_range + (ROWS / 4);
		//endexomena
		//se allon pinaka? h ston idio?
		//paradeigma mandelbrot
		//copy olo ton pinaka se local var k mutex meta sthn apothikeusi
		
	} else {
		int data[ROWS / global_num_of_threads + 1][COLUMNS / global_num_of_threads];
		from_range = thread_id * (ROWS / 4) - 1;
		to_range = from_range + (ROWS / 4) + 1;
	}
	
	for (j = 0; j < ROWS; j++) {
		for (n = 0; n < COLUMNS; n++) {
			katastasi[curr].data[j][n] =  abs((int) (katastasi[curr-1].data[j][n] + sin(7 * curr) * 3) % 6);
			
			startPosX = (j-1 < MIN_X) ? j : j-1;
			startPosY = (n-1 < MIN_Y) ? n: n-1;
			endPosX = (j+1 > MAX_X) ? j : j+1;
			endPosY = (n+1 > MAX_Y) ? n : n+1;
			
			for(rowNum = startPosX; rowNum <= endPosX; rowNum++){
				for(colNum = startPosY; colNum <= endPosY; colNum++){
					if (abs((katastasi[curr].data[j][n] - katastasi[curr-1].data[rowNum][colNum])) > 1) {
						opt = 1;
						//sum += katastasi[curr-1].data[rowNum][colNum];
						//count++;
					}
				}
			}
			if (opt == 1) {
				for(rowNum = startPosX; rowNum <= endPosX; rowNum++){
					for(colNum = startPosY; colNum <= endPosY; colNum++){
						sum += katastasi[curr-1].data[rowNum][colNum];
						count++;
					}
				}
				katastasi[curr].data[j][n]	= sum / count;
				sum = 0;
				count = 0;	
			}
			opt = 0;
			therm[curr].data[j][n] =  (22 + (sin(6.5 * curr ) * 10) - (katastasi[curr-1].data[j][n] / 4)) + (therm[curr-1].data[j][n]/50);
			entasi[curr].data[j][n] =  (entasi[curr-1].data[j][n]/4) + (sin(7 * curr) * 3);
			
			/* ftiaxnei filenames */
			if (curr >=10) {
				sprintf(buf, "%s%d", name, curr);
				//printf("%s\n", buf);

			} else {
				sprintf(buf, "%s0%d", name, curr);
				//printf("%s\n", buf);
			}
			
			/* apothikeusi */				
			file = fopen(buf,"a+"); 
			fprintf(file,"%d",katastasi[curr].data[j][n]);
			fclose(file);
		}
			
	}
	
}

int main(int argc, char *argv[]) {
	int i, cnt_problepseis;
	
	srand(time(NULL));
	init();
	
	int num_of_threads = atoi(argv[1]);
	pthread_t threads[num_of_threads];
	void *retval;              /* unused; required for join() */
	
	//pthread_mutex_init(&piLock, NULL);
	for (cnt_problepseis = 1; cnt_problepseis < HOURS; cnt_problepseis++) {
		curr = cnt_problepseis;
		global_num_of_threads = num_of_threads;
		for (i = 0; i < num_of_threads; i++) {
			pthread_create(&threads[i], NULL, &decompose_on_x, &i);
			pthread_join(threads[i], &retval);
		}
	}
	//print_debug();
	return 0;
}


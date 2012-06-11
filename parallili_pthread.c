#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#define HOURS 4
#define ROWS 8
#define COLUMNS 8
#define MAX_X 7
#define MAX_Y 7
#define MIN_X 0
#define MIN_Y 0

int curr;
int num_of_threads;

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

void read_from_binary() {
	FILE *fp;
	
	int i, j;
	
	//if((fp=fopen("map.000", "wb"))==NULL) {
		//printf("Cannot open file.\n");
	//}

	//if(fwrite(katastasi[0].data, sizeof(int), ROWS*COLUMNS, fp) != ROWS*COLUMNS)
		//printf("File write error.");
	//fclose(fp);

	/* read the values */
	if((fp=fopen("map.000", "rb"))==NULL) {
		printf("Cannot open file.\n");
	}

	if(fread(katastasi[0].data, sizeof(int), ROWS*COLUMNS, fp) != ROWS*COLUMNS) {
		if(feof(fp))
			printf("Premature end of file.");
		else
			printf("File read error.");
	}
	fclose(fp);
	/* sanity check */
	//for(j = 0; j < ROWS; j++) {
		//for(i = 0; i < COLUMNS; i++)
			//printf("%d ", katastasi[0].data[j][i]);
		//printf("\n");
	//}
}

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
				katastasi[i].data[j][n] = rand() % 5; //arxikopoihsh pinakwn katastasis me tyxaia timh apo 0 ws 5
				therm[i].data[j][n] = 22;
				entasi[i].data[j][n] = 0;
				
				//if (i == 0) {
					//file = fopen("map.000","a+"); 
					//fprintf(file,"%d", katastasi[i].data[j][n]);
					//fclose(file);
				//} 
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
	
	int from_range = thread_id * (ROWS / num_of_threads);
	int to_range = from_range + (ROWS / num_of_threads);
	
	for (j = from_range; j < to_range; j++) {
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
			/* mutex */
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
	read_from_binary();
	
	num_of_threads = atoi(argv[1]);
	pthread_t threads[num_of_threads];
	void *retval;              /* unused; required for join() */
	
	//pthread_mutex_init(&piLock, NULL);
	for (cnt_problepseis = 1; cnt_problepseis < HOURS + 1; cnt_problepseis++) {
		curr = cnt_problepseis;
		
		for (i = 0; i < num_of_threads; i++) {
			pthread_create(&threads[i], NULL, &decompose_on_x, &i);
			pthread_join(threads[i], &retval);
		}
	}
	//print_debug();
	return 0;
}


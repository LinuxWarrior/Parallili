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

int main(int argc, char *argv[]) {
	int i, j, n, startPosX, startPosY, endPosX, endPosY, rowNum, colNum;
	FILE *file; 
	
	int sum = 0;
	int count = 0;
	int opt = 0;
	
	const char *name = "map.0";
	char buf[24] = { 0 };
	
	srand(time(NULL));
	init();
	
	for (i = 1; i < HOURS; i++) {
		for (j = 0; j < ROWS; j++) {
			for (n = 0; n < COLUMNS; n++) {
				katastasi[i].data[j][n] =  abs((int) (katastasi[i-1].data[j][n] + sin(7*i) * 3) % 6);
				
				startPosX = (j-1 < MIN_X) ? j : j-1;
				startPosY = (n-1 < MIN_Y) ? n: n-1;
				endPosX = (j+1 > MAX_X) ? j : j+1;
				endPosY = (n+1 > MAX_Y) ? n : n+1;
				
				for(rowNum = startPosX; rowNum <= endPosX; rowNum++){
					for(colNum = startPosY; colNum <= endPosY; colNum++){
						if (abs((katastasi[i].data[j][n] - katastasi[i-1].data[rowNum][colNum])) > 1) {
							opt = 1;
							//sum += katastasi[i-1].data[rowNum][colNum];
							//count++;
						}
					}
				}
				if (opt == 1) {
					for(rowNum = startPosX; rowNum <= endPosX; rowNum++){
						for(colNum = startPosY; colNum <= endPosY; colNum++){
							sum += katastasi[i-1].data[rowNum][colNum];
							count++;
						}
					}
					katastasi[i].data[j][n]	= sum / count;
					sum = 0;
					count = 0;	
				}
				opt = 0;
				therm[i].data[j][n] =  (22 + (sin(6.5 * i ) * 10) - (katastasi[i-1].data[j][n] / 4)) + (therm[i-1].data[j][n]/50);
				entasi[i].data[j][n] =  (entasi[i-1].data[j][n]/4) + (sin(7 * i) * 3);
				
				/* ftiaxnei filenames */
				if (i >=10) {
					sprintf(buf, "%s%d", name, i);
					//printf("%s\n", buf);

				} else {
					sprintf(buf, "%s0%d", name, i);
					//printf("%s\n", buf);
				}
				
				/* apothikeusi */				
				file = fopen(buf,"a+"); 
				fprintf(file,"%d",katastasi[i].data[j][n]);
				fclose(file);
			}
				
		}
	}
	
	//print_debug();
	return 0;
}


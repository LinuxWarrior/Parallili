#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

#define HOURS 2  //problepseis
#define ROWS 4096
#define COLUMNS 4096
#define MAX_X 4095
#define MAX_Y 4095
#define MIN_X 0
#define MIN_Y 0

/* dhmiourgia structs isa me tis problepseis mas (HOURS) */
/* pinakas data me ta stoixeia ths katastasis, thermokrasias, entasis */
struct Kat
{
	int data[ROWS][COLUMNS];
} static katastasi[HOURS];

struct Ent
{
	float data[ROWS][COLUMNS];
} static entasi[HOURS];

struct Th
{
	int data[ROWS][COLUMNS];
} static thermokrasia[HOURS];

struct test		//dummy struct gia kapoion periergo logo pathainei segmentation fault otan to afairesw
{				//dafuq????
	int data[ROWS][COLUMNS];
} static t[HOURS];
	
/* ----------------------------------------------------------------- */

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
	/*for(j = 0; j < ROWS; j++) {
		for(i = 0; i < COLUMNS; i++)
			printf("%d ", katastasi[0].data[j][i]);
		printf("\n");
	}*/
}

void init() {
	int i, j, n;
	FILE *file; 
	
	for (i = 0; i < HOURS; i++) {
		for (j = 0; j < ROWS; j++) {
			for (n = 0; n < COLUMNS; n++) {
				katastasi[i].data[j][n] = rand() % 5; //arxikopoihsh pinakwn katastasis me tyxaia timh apo 0 ws 5
				thermokrasia[i].data[j][n] = 22;			//arxikopoihsh thermokrasias me 22
				entasi[i].data[j][n] = 0;			//........... entasi me 0
				
				//if (i == 0) { 
					//file = fopen("map.000","a+"); 
					//fprintf(file,"%d", katastasi[i].data[j][n]); //apothikeusi arxikou pinaka katastasis
					//fclose(file);								// se arxeio map.000
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

int main(int argc, char *argv[]) {
	int i, j, n, startPosX, startPosY, endPosX, endPosY, rowNum, colNum;
	FILE *file; 
	struct timeval	StartTime, EndTime;
	
	int sum = 0;
	int count = 0;
	int opt = 0;
	
	const char *name = "map.0";
	char buf[24] = { 0 };
	
	srand(time(NULL));
	init(); //arxikopoihsh
	read_from_binary();
	
	gettimeofday(&StartTime, NULL);
	/* loops gia gemisma epomenwn katastasewn */
	for (i = 0; i < HOURS; i++) {
		for (j = 0; j < ROWS; j++) {
			for (n = 0; n < COLUMNS; n++) {
				katastasi[i+1].data[j][n] =  abs((int) (katastasi[i].data[j][n] + sin(7*i) * 3) % 6);
				
				/* euresi geitonikwn stoixeiwn */
				startPosX = (j-1 < MIN_X) ? j : j-1;
				startPosY = (n-1 < MIN_Y) ? n: n-1;
				endPosX = (j+1 > MAX_X) ? j : j+1;
				endPosY = (n+1 > MAX_Y) ? n : n+1;
				
				for(rowNum = startPosX; rowNum <= endPosX; rowNum++){
					for(colNum = startPosY; colNum <= endPosY; colNum++){
						/* elegxos gia apokliseis megalyteres tou 1 */
						if (abs((katastasi[i+1].data[j][n] - katastasi[i].data[rowNum][colNum])) > 1) {
							opt = 1; //flag gia apoklish
						}
						/* ------------------------------------------ */
					}
				}
				/* -------------------------------------------------- */
				/* an exoume megalyterh apoklish apo 1 */
				/* briskoume to meso oro twn geitonikwn stoixeiwn */
				/* kai antikathistoume thn timh */ 
				if (opt == 1) {
					for(rowNum = startPosX; rowNum <= endPosX; rowNum++){
						for(colNum = startPosY; colNum <= endPosY; colNum++){
							sum += katastasi[i].data[rowNum][colNum];
							count++;
						}
					}
					katastasi[i+1].data[j][n]	= sum / count;
					sum = 0;
					count = 0;	
				}
				/* _________________________________________________ */
				opt = 0;
				thermokrasia[i+1].data[j][n] =  (22 + (sin(6.5 * i ) * 10) - (katastasi[i].data[j][n] / 4)) + (thermokrasia[i].data[j][n]/50);
				entasi[i+1].data[j][n] =  (entasi[i].data[j][n]/4) + (sin(7 * i) * 3);
				
				/* ftiaxnei filenames */
				//if (i >=10) {
					//sprintf(buf, "%s%d", name, i);
					////printf("%s\n", buf);

				//} else {
					//sprintf(buf, "%s0%d", name, i);
					////printf("%s\n", buf);
				//}
				
				///* apothikeusi */				
				//file = fopen(buf,"a+"); 
				//fprintf(file,"%d",katastasi[i].data[j][n]);
				//fclose(file);
			}
				
		}
	}
	
	gettimeofday(&EndTime, NULL);

	if (EndTime.tv_usec < StartTime.tv_usec) {
		int nsec = (StartTime.tv_usec - EndTime.tv_usec) / 1000000 + 1;
		StartTime.tv_usec -= 1000000 * nsec;
		StartTime.tv_sec += nsec;
	}
	if (EndTime.tv_usec - StartTime.tv_usec > 1000000) {
		int nsec = (EndTime.tv_usec - StartTime.tv_usec) / 1000000;
		StartTime.tv_usec += 1000000 * nsec;
		StartTime.tv_sec -= nsec;
	}

	printf("\n\nCalculation time: %ld.%.6ld seconds\n", EndTime.tv_sec  - StartTime.tv_sec, EndTime.tv_usec - StartTime.tv_usec);
	//print_debug();
	return 0;
}

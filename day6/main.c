#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_RACES 3
#define MAX_LINE_LENGTH 50

typedef struct {
  int time;
  int distance;
} Race;

Race **get_races(FILE *fptr) {
  Race **races = malloc(NUM_RACES * sizeof(Race*));

  char times[MAX_LINE_LENGTH];
  char distances[MAX_LINE_LENGTH];

  // get lines from file
  fgets(times, MAX_LINE_LENGTH, fptr);
  fgets(distances, MAX_LINE_LENGTH, fptr);  

  char *time_ch = &times[0];
  char *distance_ch = &distances[0];

  // move char pointers to first number
  while (!isdigit(*time_ch)) time_ch++;
  while (!isdigit(*distance_ch)) distance_ch++;

  int race_index = 0;

  while (1) {
    races[race_index] = malloc(sizeof(Race));
    
    races[race_index]->time = strtol(time_ch, &time_ch, 10);
    races[race_index]->distance = strtol(distance_ch, &distance_ch, 10);

    if (*time_ch == '\n') break;

    time_ch++;
    distance_ch++;

    race_index++;
  }
  
  rewind(fptr);

  return races;
}

int main() {
  FILE *fptr;
  fptr = fopen("input.txt", "r");

  Race **races = get_races(fptr);

  // free races memory
  int index = 0;
  while (races[index] != NULL) {
    printf("Race %d: %d - %d\n", index, races[index]->time, races[index]->distance);

    free(races[index]);
    index++;
  }

  free(races);

  fclose(fptr);

  return 0;
}

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_RACES 3
#define MAX_LINE_LENGTH 50

typedef struct {
  unsigned long long int time;
  unsigned long long int distance;
} Race;

// for part one
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

// for part two
Race *get_race(FILE *fptr) {
  Race *race = malloc(sizeof(Race));

  char times[MAX_LINE_LENGTH];
  char distances[MAX_LINE_LENGTH];

  // get lines from file
  fgets(times, MAX_LINE_LENGTH, fptr);
  fgets(distances, MAX_LINE_LENGTH, fptr);  

  char *time_ch = &times[0];
  char *distance_ch = &distances[0];

  char time[MAX_LINE_LENGTH] = {'\0'};
  char distance[MAX_LINE_LENGTH] = {'\0'};

  while (*time_ch != '\n') {
    if (isdigit(*time_ch)) strncat(time, time_ch, 1);
    time_ch++;
  }

  while (*distance_ch != '\n') {
    if (isdigit(*distance_ch)) strncat(distance, distance_ch, 1);
    distance_ch++;
  }

  race->time = strtol(time, NULL, 10);
  race->distance = strtol(distance, NULL, 10); 

  rewind(fptr);
  return race;
}

unsigned long long int find_combinations(Race *race) {
  unsigned long long int sum = 0;

  for (unsigned long long int i = 1; i < race->time; i++) {
    unsigned long long int distance = i * (race->time - i);
    if (distance > race->distance) sum++; 
  }

  return sum;
}

int main() {
  FILE *fptr;
  fptr = fopen("input.txt", "r");

  Race **races = get_races(fptr);

  int product = 1;

  // free races memory
  int index = 0;
  while (races[index] != NULL) {
    product *= find_combinations(races[index]);

    free(races[index]);
    index++;
  }

  // for part 2
  Race *race = get_race(fptr);

  printf("Product: %d, Product part two: %llu\n", product, find_combinations(race));

  free(races);
  free(race);

  fclose(fptr);

  return 0;
}

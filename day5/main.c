#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
  unsigned long int value;
  unsigned long int destination;
} Seed; 

int find_num_seeds(FILE *fptr) {
  int total = 0;
  char ch = '\0';

  while (ch != '\n') {
    ch = getc(fptr);
    if (ch == ' ') total++;
  }

  rewind(fptr);
  return total;
}

int find_line_length(FILE *fptr) {
  int total = 0;
  char ch = '\0'; 

  while (ch != '\n') {
    ch = getc(fptr);
    total++;
  }

  fseek(fptr, -total, SEEK_CUR); 
  return total;
}

Seed **get_seeds(FILE *fptr) {
  int num_seeds = find_num_seeds(fptr);
  Seed **seeds = malloc(num_seeds * sizeof(Seed*));

  int line_length = find_line_length(fptr);
  char line[line_length + 1]; 

  int seed_index = 0;
  fgets(line, line_length + 1, fptr);

  char *ch = &line[0];
  while (*ch != '\n') {
    if (!isdigit(*ch)) {
      ch++;
      continue;
    }
    
    unsigned long int seed = strtol(ch, &ch, 10);
    
    seeds[seed_index] = malloc(sizeof(Seed)); 
    seeds[seed_index]->value = seed;
    seeds[seed_index]->destination = seed;

    seed_index++;
  }

  rewind(fptr);
  return seeds;
}

int next_map(FILE *fptr) {
  char line[300];

  while (fgets(line, 300, fptr)) {
    int line_index = 0; 
    while (line[line_index] != '\n') {
      if (line[line_index] == ':') return 1;
      line_index++;
    }
  }

  return 0;
}

unsigned long int find_destination(unsigned long int source, char *map_line) {
  unsigned long int map_values[3 * sizeof(unsigned long int)];  
  int map_index = 0;

  char *ch = &map_line[0];  
  while (*ch != '\n') {
    if (!isdigit(*ch)) {
      ch++;
      continue;
    } 

    map_values[map_index] = strtol(ch, &ch, 10);
    map_index++;
  }
  
  if (source < map_values[1] || source > map_values[1] + map_values[2] - 1)
    return 0;  

  return map_values[0] + (source - map_values[1]); 
}

void find_location(FILE *fptr, Seed *seed) {
  while (next_map(fptr)) {
    char line[300];

    while (fgets(line, 300, fptr)) {
      if (!isdigit(line[0])) break;

      unsigned long int destination = find_destination(seed->destination, line); 

      if (!destination) continue;
      seed->destination = destination;
      break;
    } 
  } 
  
  printf("%lu -> %lu\n", seed->value, seed->destination);
  rewind(fptr);
} 

int main() {
  FILE *fptr;
  fptr = fopen("input.txt", "r");

  Seed **seeds = get_seeds(fptr);
  int num_seeds = find_num_seeds(fptr);

  unsigned long int lowest_location = -1;

  for (int i = 0; i < num_seeds; i++) {
    find_location(fptr, seeds[i]); 
    if (seeds[i]->destination < lowest_location)
      lowest_location = seeds[i]->destination;
  }

  printf("Lowest location: %lu\n", lowest_location);

  // free allocated memory
  free(seeds);
  fclose(fptr);
}

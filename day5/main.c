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

    seed_index++;
  }

  rewind(fptr);
  return seeds;
}

int find_destination(unsigned long int source, char *map_line) {

  return 0;
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

void find_location(FILE *fptr, Seed *seed) {
  while (next_map(fptr)) {
    char line[300];

    while (fgets(line, 300, fptr)) {
      if (!isdigit(line[0])) break;
      printf("%c\n", line[0]);
    } 
  } 
  
  printf("Finished one seed: %lu\n", seed->value);
  rewind(fptr);
} 

int main() {
  FILE *fptr;
  fptr = fopen("input.txt", "r");

  // after get_seeds the file pointer is at the second line
  Seed **seeds = get_seeds(fptr);
  int num_seeds = find_num_seeds(fptr);

  for (int i = 0; i < num_seeds; i++)
    find_location(fptr, seeds[i]); 

  // free allocated memory
  free(seeds);
  fclose(fptr);
}

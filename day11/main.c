#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_GALAXIES 1000
#define MAX_LINE_LENGTH 1000
#define MAX_PAIRS 1000 

typedef struct {
  int row, column;
} Galaxy;

typedef struct {
  int first, second;
} Pair;

bool check_column(FILE *fptr, int column) {
  long int offset = ftell(fptr);
  rewind(fptr);

  char line[MAX_LINE_LENGTH];
  bool no_galaxies = true;
 
  while (fgets(line, MAX_LINE_LENGTH, fptr)) {
    if (line[column] == '.') continue;
    no_galaxies = false;
    break; 
  } 
  
  fseek(fptr, offset, SEEK_SET);

  return no_galaxies;
}

Galaxy **get_galaxies(char *file) {
  FILE *fptr = fopen(file, "r");

  Galaxy **galaxies = malloc(MAX_GALAXIES * sizeof(Galaxy*));

  char line[MAX_LINE_LENGTH];
  int row = 0;
  int galaxy_index = 0;

  while (fgets(line, MAX_LINE_LENGTH, fptr)) {
    int column = 0; 
    int index = 0;

    bool no_galaxies = true;
    
    while (line[index] != '\n') {
      if (line[index] == '.') {
        check_column(fptr, index)
          ? column += 2
          : column++;  
      } else {
        galaxies[galaxy_index] = malloc(sizeof(Galaxy));

        galaxies[galaxy_index]->row = row;
        galaxies[galaxy_index]->column = column;

        no_galaxies = false;
        
        column++;
        galaxy_index++;
      }
      
      index++; 
    }

    no_galaxies
      ? row += 2
      : row++;
  }

  galaxies = realloc(galaxies, (galaxy_index + 1) * sizeof(Galaxy*));
  galaxies[galaxy_index] = NULL;

  fclose(fptr);
  return galaxies;
}

void free_galaxies(Galaxy **galaxies) {
  int index = 0;
  while (galaxies[index] != NULL) {
    free(galaxies[index]);
    index++;
  }

  free(galaxies);
}

int find_distance(Galaxy *one, Galaxy *two) {
  int horizontal = abs(one->column - two->column);
  int vertical = abs(one->row - two->row);

  return horizontal + vertical;
}

bool find_pair(Pair *pairs, int first, int second, int pair_index) {
  for (int i = 0; i < pair_index; i++) {
    if ((first == pairs[i].first && second == pairs[i].second) 
      || (second == pairs[i].first && first == pairs[i].second))
      return true;
  }
  
  return false;
}

int main() {
  Galaxy **galaxies = get_galaxies("input.txt");

  int sum = 0;
 
  Pair *pairs = calloc(MAX_PAIRS, sizeof(Pair));
  int pair_index = 0;

  int first = 0;

  while (galaxies[first] != NULL) {
    int second = 0;
    
    while (galaxies[second] != NULL) {
      Pair pair = {first, second};

      if (!find_pair(pairs, first, second, pair_index) && first != second) {
        
        sum += find_distance(galaxies[first], galaxies[second]);
        pairs[pair_index] = pair;
        pair_index++;
      }
 
      second++;
    }

    first++;
  }

  printf("%d\n", pair_index);

  printf("Sum: %d\n", sum);

  free(pairs);
  free_galaxies(galaxies);

  return 0;
}

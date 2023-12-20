#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1000
#define MAX_LINE_NUM 500

int **get_histories(char *file) {
  FILE *fptr = fopen(file, "r");

  int **histories = malloc(MAX_LINE_NUM * sizeof(int*));

  char *line = malloc(MAX_LINE_LENGTH);
  int index = 0;

  while (fgets(line, MAX_LINE_LENGTH, fptr)) {
    histories[index] = malloc(MAX_LINE_LENGTH * sizeof(int));
    char *ch = &line[0];

    int history = 1;
    while (*ch != '\n') {
      histories[index][history] = strtol(ch + (history != 1 ? 1 : 0), &ch, 10); 
      history++;
    }

    // reallocate and set first element to the length of the array
    histories[index] = realloc(histories[index], history * sizeof(int));
    histories[index][0] = history;

    index++;
  }  

  histories = realloc(histories, (index + 1) * sizeof(int*));
  histories[index] = NULL; 

  fclose(fptr);

  return histories;
}

int *reduce_history(int *history) {
  int length = history[0];
  int *reduced_history = malloc((length - 1) * sizeof(int));
  reduced_history[0] = length - 1;

  bool all_zeros = true;
 
  for (int i = 2; i < length; i++) {
    reduced_history[i - 1] = history[i] - history[i - 1];     
    if (reduced_history[i - 1]) all_zeros = false;
  }

  if (!all_zeros) return reduced_history;
 
  free(reduced_history);
  return NULL;
}

int next_value(int *history) {
  int length = history[0];
  int *reduced_history = reduce_history(history);

  int value = reduced_history == NULL
    ? 0
    : reduced_history[length - 2] + next_value(reduced_history); 

  free(reduced_history);

  return value;
}

int prev_value(int *history) {
  int length = history[0];
  int *reduced_history = reduce_history(history);

  int value = reduced_history == NULL
    ? 0
    : reduced_history[1] - prev_value(reduced_history);
  
  free(reduced_history);

  return value; 
}

void free_histories(int **histories) {
  int index = 0;
  while (histories[index] != NULL) {
    free(histories[index]);
    index++;
  }
  free(histories);
}

int main() {
  int **histories = get_histories("input.txt");

  int next = 0, prev = 0;

  int index = 0; 
  while (histories[index] != NULL) {
    int length = histories[index][0];
    next += next_value(histories[index]) + histories[index][length - 1];
    prev += histories[index][1] - prev_value(histories[index]); 
    index++;
  }

  printf("Next: %d, Prev: %d\n", next, prev);

  // free histories
  free_histories(histories);

  return 0;
}

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

int predict_value(int *history) {
  int length = history[0];
  int *reduced_history = malloc((length - 1) * sizeof(int));
  reduced_history[0] = length - 1;

  bool all_zeros = true;
 
  for (int i = 2; i < length; i++) {
    reduced_history[i - 1] = history[i] - history[i - 1];     
    if (reduced_history[i - 1]) all_zeros = false;
  }

  int predicted_value = all_zeros
    ? 0
    : reduced_history[length - 2] + predict_value(reduced_history); 

  free(reduced_history);

  return predicted_value;
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

  int sum = 0;

  int index = 0; 
  while (histories[index] != NULL) {
    int length = histories[index][0];
    sum += predict_value(histories[index]) + histories[index][length - 1];
    index++;
  }

  printf("Sum: %d\n", sum);

  // free histories
  free_histories(histories);

  return 0;
}

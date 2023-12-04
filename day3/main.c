#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

int get_line_num(FILE *fptr) {
  int total = 0;
  char ch;

  while (!feof(fptr)) {
    ch = fgetc(fptr);
    if (ch == '\n') total++;
  }
  
  rewind(fptr);
  return total;
}

size_t get_line_length(FILE *fptr, size_t max_size) {
  char str[max_size];
  fgets(str, max_size, fptr);

  rewind(fptr);
  return strlen(str);
}

void free_lines(char **lines, int num_lines) {
  for (int i = 0; i < num_lines; i++) {
    free(lines[i]);
  }
  free(lines);
}

int main() {
  FILE *fptr;
  fptr = fopen("input.txt", "r");
  
  int num_lines = get_line_num(fptr);
  size_t line_length = get_line_length(fptr, 300);

  char **lines = malloc(num_lines * sizeof(char*));
 
  for (int i = 0; i < num_lines; i++) {
    lines[i] = malloc(line_length + 1);
    fgets(lines[i], line_length + 1, fptr);
  }  
  
  for (int i = 0; i < num_lines; i++) {
    printf("%s", lines[i]);
  }
 
  free_lines(lines, num_lines);
  fclose(fptr);
}

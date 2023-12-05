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

typedef struct {
  int ratio;
  int num_parts;
  int x, y;
} Gear;

int add_gear(Gear **gears) {
  int index = 0;
  while (gears[index] != NULL) {
    index++;
  }
  gears[index] = malloc(sizeof(Gear));

  gears[index]->ratio = 1;
  gears[index]->num_parts = 0;
  gears[index]->x = 0;
  gears[index]->y = 0;

  return index;
}

int is_symbol(const char *ch) {
  return !isdigit(*ch) && !isalpha(*ch) && *ch != '.';
}

int is_valid(
  char **lines, 
  int *line_index, 
  int *digit_index, 
  int *digit_length, 
  int *num_lines, 
  size_t *line_length) {

  int is_top = *line_index == 0;
  int is_bottom = *line_index == *num_lines - 1;
  int is_left = *digit_index == 0;
  int is_right = (*digit_index + *digit_length) == *line_length - 1;

  // check directly above digit
  if (!is_top) {
    for (int i = *digit_index; i < *digit_index + *digit_length; i++) {
      const char *ch = &lines[*line_index - 1][i];
      if (is_symbol(ch)) return 1;
    }    
  } 

  // check directly under digit
  if (!is_bottom) {
    for (int i = *digit_index; i <*digit_index + *digit_length; i++) {
      const char *ch = &lines[*line_index + 1][i];
      if (is_symbol(ch)) return 1; 
    }
  } 

  // check directly to the left of digit
  if (!is_left) {
    const char *ch = &lines[*line_index][*digit_index - 1];
    if (is_symbol(ch)) return 1;
  } 

  // check directly to the right of digit
  if (!is_right) {
    const char *ch = &lines[*line_index][*digit_index + *digit_length];
    if (is_symbol(ch)) return 1;
  } 

  // check top left corner
  if (!is_left && !is_top) {
    const char *ch = &lines[*line_index - 1][*digit_index - 1];
    if (is_symbol(ch)) return 1;
  }

  // check top right corner
  if (!is_top && !is_right) {
    const char *ch = &lines[*line_index - 1][*digit_index + *digit_length];
    if (is_symbol(ch)) return 1;
  }
 
  // check bottom left corner
  if (!is_left && !is_bottom) {
    const char *ch = &lines[*line_index + 1][*digit_index - 1];
    if (is_symbol(ch)) return 1;
  }

  // check bottom right corner
  if (!is_bottom && !is_right) {
    const char *ch = &lines[*line_index + 1][*digit_index +*digit_length];
    if (is_symbol(ch)) return 1;
  }

  return 0;
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

  int sum = 0;
  Gear **gears = malloc(200 * sizeof(Gear*)); 
  char **lines = malloc(num_lines * sizeof(char*));
  
  for (int i = 0; i < num_lines; i++) {
    lines[i] = malloc(line_length + 1);
    fgets(lines[i], line_length + 1, fptr);
  }  
  
  for (int i = 0; i < num_lines; i++) {
    int digit_index = 0;
    char *ch = lines[i];

    while (*ch != '\n') {
      if (!isdigit(*ch)) {
        ch++;
        digit_index++;
        continue;
      }
      char *old_ch = ch;
      int num = strtol(ch, &ch, 10);
      int digit_length = ch - old_ch;

      if (is_valid(lines, &i, &digit_index, &digit_length, &num_lines, &line_length)) sum += num;

      digit_index += digit_length;
    }
  }

  printf("Sum: %d\n", sum);

  // free gears memory
  for (int i = 0; i < 200; i++) {
    if (gears[i] != NULL) {
      free(gears[i]);
    }
  }  
  free(gears);

  free_lines(lines, num_lines);
  fclose(fptr);
}

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

Gear *add_gear(Gear **gears) {
  int index = 0;
  while (gears[index] != NULL) {
    index++;
  }
  gears[index] = malloc(sizeof(Gear));

  gears[index]->ratio = 1;
  gears[index]->num_parts = 0;
  gears[index]->x = 0;
  gears[index]->y = 0;

  return gears[index];
}

Gear *find_gear(Gear **gears, int x, int y) {
  int index = 0;
  while (gears[index] != NULL) {
    if (gears[index]->x == x && gears[index]->y == y) return gears[index];
    index++; 
  }
  return NULL;
}

void update_gears(Gear **gears, int x, int y, int *num) {
  Gear *gear = find_gear(gears, x, y);
  if (gear == NULL) {
    gear = add_gear(gears);
    gear->x = x;
    gear->y = y;
  } 
  gear->ratio *= *num;
  gear->num_parts++;        
}

int is_symbol(const char *ch) {
  return !isdigit(*ch) && !isalpha(*ch) && *ch != '.';
}

int is_valid(
  char **lines, 
  Gear **gears,
  int *line_index, 
  int *digit_index, 
  int *digit_length, 
  int *num_lines, 
  int *num,
  size_t *line_length) {

  int is_top = *line_index == 0;
  int is_bottom = *line_index == *num_lines - 1;
  int is_left = *digit_index == 0;
  int is_right = (*digit_index + *digit_length) == *line_length - 1;

  int valid = 0;

  // check directly above digit
  if (!is_top) {
    for (int i = *digit_index; i < *digit_index + *digit_length; i++) {
      const char *ch = &lines[*line_index - 1][i];
      if (is_symbol(ch)) valid = 1;
      if (*ch == '*') update_gears(gears, i, *line_index - 1, num); 
    }    
  } 

  // check directly under digit
  if (!is_bottom) {
    for (int i = *digit_index; i <*digit_index + *digit_length; i++) {
      const char *ch = &lines[*line_index + 1][i];
      if (is_symbol(ch)) valid = 1;
      if (*ch == '*') update_gears(gears, i, *line_index + 1, num); 
    }
  } 

  // check directly to the left of digit
  if (!is_left) {
    const char *ch = &lines[*line_index][*digit_index - 1];
    if (is_symbol(ch)) valid = 1;
    if (*ch == '*') update_gears(gears, *digit_index - 1, *line_index, num); 
  } 

  // check directly to the right of digit
  if (!is_right) {
    const char *ch = &lines[*line_index][*digit_index + *digit_length];
    if (is_symbol(ch)) valid = 1;
    if (*ch == '*') update_gears(gears, *digit_index + *digit_length, *line_index, num); 
  } 

  // check top left corner
  if (!is_left && !is_top) {
    const char *ch = &lines[*line_index - 1][*digit_index - 1];
    if (is_symbol(ch)) valid = 1;
    if (*ch == '*') update_gears(gears, *digit_index - 1, *line_index - 1, num); 
  }

  // check top right corner
  if (!is_top && !is_right) {
    const char *ch = &lines[*line_index - 1][*digit_index + *digit_length];
    if (is_symbol(ch)) valid = 1;
    if (*ch == '*') update_gears(gears, *digit_index + *digit_length, *line_index - 1, num); 
  }
 
  // check bottom left corner
  if (!is_left && !is_bottom) {
    const char *ch = &lines[*line_index + 1][*digit_index - 1];
    if (is_symbol(ch)) valid = 1;
    if (*ch == '*') update_gears(gears, *digit_index - 1, *line_index + 1, num); 
  }

  // check bottom right corner
  if (!is_bottom && !is_right) {
    const char *ch = &lines[*line_index + 1][*digit_index + *digit_length];
    if (is_symbol(ch)) valid = 1;
    if (*ch == '*') update_gears(gears, *digit_index + *digit_length, *line_index + 1, num); 
  }

  return valid;
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
  int gear_sum = 0;

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

      if (
        is_valid(
          lines, 
          gears, 
          &i, 
          &digit_index, 
          &digit_length, 
          &num_lines, 
          &num, 
          &line_length)
        ) sum += num;

      digit_index += digit_length;
    }
  }

  int index = 0;
  while (gears[index] != NULL) {
    if (gears[index]->num_parts == 2) gear_sum += gears[index]->ratio;
    index++;
  }

  printf("Sum: %d, Gear sum: %d\n", sum, gear_sum);

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

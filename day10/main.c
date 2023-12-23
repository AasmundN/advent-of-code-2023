#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LINE_NUM 1000
#define MAX_LINE_LENGTH 1000

typedef enum {
  NORTH,
  EAST,
  SOUTH,
  WEST,
} Direction;

typedef struct {
  int row, column;
  int distance;
  Direction end;
} Pipe;

char **get_grid(char *file) {
  FILE *fptr = fopen(file, "r");

  char **grid = malloc(MAX_LINE_NUM * sizeof(char*));
  char line[MAX_LINE_LENGTH];

  int row = 0;
  long int prev_line_end = 0;

  while (fgets(line, MAX_LINE_LENGTH, fptr)) {
    long int line_end = ftell(fptr);

    grid[row] = malloc(line_end - prev_line_end);
    grid[row][0] = '\0';

    strncpy(grid[row], line, line_end - prev_line_end - 1); 

    prev_line_end = line_end;
    row++;
  }

  grid = realloc(grid, (row + 1) * sizeof(char*));
  grid[row] = NULL;

  fclose(fptr);
  return grid;
}

void free_grid(char **grid) {
  int index = 0;
  while (grid[index] != NULL) {
    free(grid[index]);
    index++;
  } 
  free(grid);
}

Direction next_direction(char pipe, Direction start) {
  Direction end;

  switch (start) {
    case SOUTH:
      if (pipe == '|') end = SOUTH;
      if (pipe == 'J') end = WEST;
      if (pipe == 'L') end = EAST;
      break;

    case WEST:
      if (pipe == 'L') end = NORTH;
      if (pipe == '-') end = WEST;
      if (pipe == 'F') end = SOUTH;
      break;

    case NORTH:
      if (pipe == '7') end = WEST;
      if (pipe == '|') end = NORTH;
      if (pipe == 'F') end = EAST;
      break;

    case EAST:
      if (pipe == 'J') end = NORTH;
      if (pipe == '-') end = EAST;
      if (pipe == '7') end = SOUTH;
      break;
  }

  return end;
}

int row_shift(Direction direction) {
  switch (direction) {
    case NORTH:
      return -1;
 
    case SOUTH:
      return 1;

    default:
      return 0;
  }
}

int column_shift(Direction direction) {
  switch (direction) {
    case WEST:
      return -1;

    case EAST:
      return 1;

    default:
      return 0;
  }
}

bool pipe_connected(char pipe, Direction direction) {
  if (pipe == '.') return false;
  
  switch (direction) {
    case NORTH:
      return pipe == '7' || pipe == '|' || pipe == 'F';

    case EAST:
      return pipe == '7' || pipe == '-' || pipe == 'J';

    case SOUTH:
      return pipe == 'J' || pipe == '|' || pipe == 'L';

    case WEST:
      return pipe == 'F' || pipe == '-' || pipe == 'L';
  }
} 

Pipe *get_starting_pipes(char **grid, char **tiles) {
  // position of S
  int row = 0;
  int column = 0;

  // find position of S
  while (grid[row] != NULL) {
    bool done = false;
    column = 0;

    while (grid[row][column] != '\0') {
      if (grid[row][column] == 'S') {
        done = true;
        break;
      }
      column++;
    }

    if (done) break;
    row++;
  }

  tiles[row][column] = 'F';

  Pipe *starting_pipes = malloc(2 * sizeof(Pipe));

  // find starting pipes
  int pipe_index = 0; 
 
  for (int direction = NORTH; direction <= WEST; direction++) {
    if (pipe_index > 1) break;

    int x = column + column_shift(direction);
    int y = row + row_shift(direction);

    if (!pipe_connected(grid[y][x], direction)) continue;
     
    starting_pipes[pipe_index].row = y;
    starting_pipes[pipe_index].column = x;
    starting_pipes[pipe_index].distance = 1;
    starting_pipes[pipe_index].end = next_direction(grid[y][x], direction);

    pipe_index++;
  }  

  return starting_pipes;
}

void update_pipe(char **grid, char **tiles, Pipe *pipe) {
  tiles[pipe->row][pipe->column] = grid[pipe->row][pipe->column];

  Direction entry = pipe->end;
  int row = pipe->row + row_shift(entry);
  int column = pipe->column + column_shift(entry);

  pipe->row = row;
  pipe->column = column;
  pipe->distance++;
  pipe->end = next_direction(grid[row][column], entry); 
}
 
int get_rows(char **grid) {
  int index = 0;
  while(grid[index] != NULL) index++;
  
  return index;
}

int get_columns(char **grid) {
  char *ch = grid[0];
  while (*ch != '\0') ch++;

  return ch - grid[0];
}

char **get_tiles(int rows, int columns) {
  char **tiles = malloc((rows + 1) * sizeof(char*));

  for (int i = 0; i < rows; i++) 
    tiles[i] = calloc(columns + 1, sizeof(char));

  return tiles;
}

void free_tiles(char **tiles) {
  int index = 0;
  while (tiles[index] != NULL) {
    free(tiles[index]);
    index++;
  }
  free(tiles);
}

int count_north(char **tiles, int start_row, int column, int rows) {
  int count = 0; 

  bool passed_loop = false;
  char pending = '\0';
  
  int row = start_row;

  while (true) {
    char tile = tiles[row][column];

    if (tile != '\0') passed_loop = true;
    
    switch(tile) {
      case '-':
        count++;
        break;
     
      case 'L':
        pending = 'L';
        break;

      case 'J':
        pending = 'J';
        break;

      case '7':
        if (pending == 'L') {
          pending = '\0';
          count++;
        } else if (pending == 'J') {
          pending = '\0';
        }
        break;

      case 'F':
        if (pending == 'J') {
          pending = '\0';
          count++;
        } else if (pending == 'L') {
          pending = '\0';
        }
        break;
    }

    if (row - 1 < 0) break;
     
    row--;  
  } 

  return count; 
}

int count_south(char **tiles, int start_row, int column, int rows) {
  int count = 0; 

  bool passed_loop = false;
  char pending = '\0';
  
  int row = start_row;

  while (true) {
    char tile = tiles[row][column];

    if (tile != '\0') passed_loop = true;
    
    switch(tile) {
      case '-':
        count++;
        break;
     
      case '7':
        pending = '7';
        break;

      case 'F':
        pending = 'F';
        break;

      case 'L':
        if (pending == '7') {
          pending = '\0';
          count++;
        } else if (pending == 'F') {
          pending = '\0';
        }
        break;

      case 'J':
        if (pending == 'F') {
          pending = '\0';
          count++;
        } else if (pending == '7') {
          pending = '\0';
        }
        break;
    }

    if (row + 1 > rows - 1) break;
     
    row++;  
  } 

  return count; 
}

int count_east(char **tiles, int row, int start_column, int columns) {
  int count = 0; 

  bool passed_loop = false;
  char pending = '\0';
  
  int column = start_column;

  while (true) {
    char tile = tiles[row][column];

    if (tile != '\0') passed_loop = true;

    switch (tile) {
      case '|':
        count++;
        break;

      case 'L':
        pending = 'L';
        break;
 
      case 'F':
        pending = 'F';
        break;
 
      case 'J':
        if (pending == 'F') {
          pending = '\0';
          count++;
        } else if (pending == 'L') {
          pending = '\0';
        }
        break;

      case '7':
        if (pending == 'L') {
          pending = '\0';
          count++;
        } else if (pending == 'F') {
          pending = '\0';
        }
    } 
    
    if (column + 1 > columns - 1) break;
    column++;
  } 

  return count; 
}

int count_west(char **tiles, int row, int start_column, int columns) {
  int count = 0; 

  bool passed_loop = false;
  char pending = '\0';

  int column = start_column;

  while (true) {
    char tile = tiles[row][column];

    if (tile != '\0') passed_loop = true;

    switch (tile) {
      case '|':
        count++;
        break;

      case 'J':
        pending = 'J';
        break;
 
      case '7':
        pending = '7';
        break;
 
      case 'F':
        if (pending == 'J') {
          pending = '\0';
          count++;
        } else if (pending == '7') {
          pending = '\0';
        }
        break;

      case 'L':
        if (pending == '7') {
          pending = '\0';
          count++;
        } else if (pending == 'J') {
          pending = '\0';
        }
    } 
    
    if (column - 1 < 0) break;
    column--;
  }

  return count; 
}

bool is_enclosed(char **tiles, int row, int column, int rows, int columns) {
  bool north = count_north(tiles, row, column, rows) % 2 == 0;
  bool south = count_south(tiles, row, column, rows) % 2 == 0;
  bool east = count_east(tiles, row, column, columns) % 2 == 0;
  bool west = count_west(tiles, row, column, columns) % 2 == 0;

  if (north || south || east || west) return false;

  return true;
}

int main() {
  char **grid = get_grid("input.txt");

  int rows = get_rows(grid);
  int columns = get_columns(grid);

  char **tiles = get_tiles(rows, columns);
  
  Pipe *pipes = get_starting_pipes(grid, tiles);

  while (pipes[0].row != pipes[1].row || pipes[0].column != pipes[1].column) {
    update_pipe(grid, tiles, pipes);
    update_pipe(grid, tiles, pipes + 1);
  }
 
  tiles[pipes[0].row][pipes[0].column] = grid[pipes[0].row][pipes[0].column];

  int sum = 0;

  // print tiles
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < columns; j++) {
      if (tiles[i][j] != '\0') continue;
      sum += is_enclosed(tiles, i, j, rows, columns);
    }

  printf("Distance: %d, Sum: %d\n", pipes[0].distance, sum);

  free(pipes);
  free_grid(grid);
  free_tiles(tiles);

  return 0;
}

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_SEED_RANGES 10
int level = 0;

typedef struct {
  unsigned long int start;
  unsigned long int length;
} Range;

typedef struct {
  unsigned long int source;
  unsigned long int destination;
  unsigned long int length;
} Map;

Range **get_seed_ranges(FILE *fptr) {
  Range **ranges = malloc(NUM_SEED_RANGES * sizeof(Range*));

  char line[500];
  fgets(line, 500, fptr);

  int range_index = 0;
  char *ch = &line[0];

  while (*ch != '\n') {
    if (!isdigit(*ch)) {
      ch++;
      continue;
    }
    
    ranges[range_index] = malloc(sizeof(Range));
    ranges[range_index]->start = strtol(ch, &ch, 10);
    ranges[range_index]->length = strtol(ch + 1, &ch, 10);

    range_index++;
  }

  return ranges;
}

Map **get_next_maps(FILE *fptr) {
  Map **maps = malloc(50 * sizeof(Map*));

  // move file pointer to start of next map
  char ch = fgetc(fptr);   
  while (ch != ':') {
    ch = fgetc(fptr);
  }

  fseek(fptr, 1, SEEK_CUR);

  // get maps
  char line[100];
  int map_index = 0;
  while (fgets(line, 100, fptr)) {
    if (!isdigit(line[0])) break;

    char *ch = &line[0];
 
    maps[map_index] = malloc(sizeof(Map));
    maps[map_index]->destination = strtol(ch, &ch, 10); 
    maps[map_index]->source = strtol(ch + 1, &ch, 10); 
    maps[map_index]->length = strtol(ch + 1, &ch, 10); 
   
    map_index++;
  }

  return maps;
}

Range **get_reduced_ranges(Range *range, Map **maps) {
  Range **reduced_ranges = malloc(100 * sizeof(Range*));

  // start and top of the starting range
  // range_start will increase as the starting range is split up
  unsigned long int range_start = range->start;
  unsigned long int range_top = range->start + range->length;

  int range_index = 0;

  // chip off bits from the starting range until it is gone
  while (range_start < range_top) { 
    reduced_ranges[range_index] = malloc(sizeof(range));

    int map_index = 0;
    int closest_map = 0;

    int all_overshoot = 1;

    // check maps
    while (maps[map_index] != NULL) {
      unsigned long int start = maps[map_index]->source;
      unsigned long int end = start + maps[map_index]->length; 

      int overshoot = range_start > end;
      int undershoot = range_start < start;

      if (!overshoot) all_overshoot = 0;

      // if the range_start is under the map start, we want to know which map is closest
      int is_closest = start - range_start < maps[closest_map]->source - range_start; 
      if (undershoot && is_closest) closest_map = map_index; 

      // range start is outside of map
      if (undershoot || overshoot) {
        map_index++; 
        continue;
      }

      // range start is inside of map
      reduced_ranges[range_index]->start = maps[map_index]->destination + (range_start - start);

      // we are done
      if (end > range_top) {
        reduced_ranges[range_index]->length = range_top - range_start;
        range_start = range_top;
        break;
      }

      // range and map completely overlap
      reduced_ranges[range_index]->length 
        = maps[map_index]->length - (range_start - maps[map_index]->source);

      map_index++;
    }

    // we are done, the range extends over all the maps
    if (all_overshoot) {
      reduced_ranges[range_index]->start = range_start;
      reduced_ranges[range_index]->length = range_top - range_start;  
      break;
    } 

    // range_start is outside all the maps, but there exists a map over it
    if (!reduced_ranges[range_index]->start) {
      reduced_ranges[range_index]->start = range_start;

      // the existing map is over the top of the starting range, we are done
      if (maps[closest_map]->source > range_top) {
        reduced_ranges[range_index]->length = range_top - range_start;  
        break;
      } 

      reduced_ranges[range_index]->length = maps[closest_map]->source - range_start; 
    }

    range_start += reduced_ranges[range_index]->length; 
    range_index++;    
    if (range_index > 10) break;
  }
  return reduced_ranges;
}

void reduce_range(Range *range, unsigned long int *lowest_location, FILE *fptr) {
  // save position of file pointer
  long int current_file_location = ftell(fptr);

  // get next map
  Map **maps = get_next_maps(fptr); 
  level++;
 
  // reduce range
  Range **reduced_ranges = get_reduced_ranges(range, maps);

  int index = 0;

  // recursively find lowest location
  while (reduced_ranges[index] != NULL) {
    if (!feof(fptr)) reduce_range(reduced_ranges[index], lowest_location, fptr);
    else if (reduced_ranges[index]->start < *lowest_location) *lowest_location = reduced_ranges[index]->start;

    index++;
  }

  // reset file pointer
  fseek(fptr, current_file_location, SEEK_SET);
  level--;
  
  // free maps memory
  index = 0;
  while (maps[index] != NULL) {
    free(maps[index]);
    index++;
  } 
  free(maps);

  // free reduced_ranges memory
  index = 0;
  while (reduced_ranges[index] != NULL) {
    free(reduced_ranges[index]);
    index++;
  } 
  free(reduced_ranges);
}

int main() {
  FILE *fptr;
  fptr = fopen("../input.txt", "r"); 

  Range **ranges = get_seed_ranges(fptr);
  unsigned long int lowest_location = -1;

  int index = 0;
  while (ranges[index] != NULL) {
    reduce_range(ranges[index], &lowest_location, fptr); 

    fseek(fptr, 6, SEEK_SET); 
    index++;
  } 

  printf("Lowest: %lu\n", lowest_location);

  // free ranges memory
  index = 0;
  while (ranges[index] != NULL) {
    free(ranges[index]);
    index++;
  } 
  free(ranges);

  fclose(fptr);
  
  return 0;
}

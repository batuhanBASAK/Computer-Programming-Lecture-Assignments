#include <stdio.h>
#include <stdlib.h>

#define MAP_SIZE 512 

#define INPUTFILE "input.txt"
#define OUTPUTFILE "output.txt"

void delete_island(int map[MAP_SIZE][MAP_SIZE], int x, int y, int water_level);

int main(int argc, const char *argv[])
{
  FILE *infile;
  FILE *outfile;
  int water_level;
  int map[MAP_SIZE][MAP_SIZE] = {0};

  int i, j;
  int temp;

  /* Open input file */
  infile = fopen(INPUTFILE, "r");
  if(infile == NULL)
    return 1;

  /* get water level from input file */
  fscanf(infile, "%d", &water_level);

  /* Get map from input file */ 
  for(i=0; i < MAP_SIZE; i++)
  {
    for(j=0; j < MAP_SIZE; j++)
    {
      fscanf(infile, "%d", &temp);
      map[i][j] = temp;
    }
  }

  /* close input file */
  fclose(infile);

  /* Open output file */
  outfile = fopen(OUTPUTFILE, "w");
  if(outfile == NULL)
    return 2;

  /* Search islands in map cordinate-by-coordinate */
  for(i=0; i < MAP_SIZE; i++)
  {
    for(j=0; j < MAP_SIZE; j++)
    {
      if(map[i][j] > water_level)
      {
        /* There is a new island found */

        /* Write islands coordinates to output file */
        fprintf(outfile, "%d %d\n", j, i);

        /* Delete current island from map to proceed */
        delete_island(map, j, i, water_level); 
      }
    }
  }

  /* close output file */ 
  fclose(outfile);

  return 0;
}


/* deletes, makes coordinate under water level, coordinate (x,y) in map
 * and moves next coordinate of current islands recursively until there is
 * no coordinate left for current island.
 */
void delete_island(int map[MAP_SIZE][MAP_SIZE], int x, int y, int water_level)
{
  /* delete current coordinate from island,
   * meaning make coordinate under water level
   */
  map[y][x] = water_level - 1;

  if(x > 0 && map[y][x-1] > water_level)
    delete_island(map, x-1, y, water_level);
  if(x < (MAP_SIZE-1) && map[y][x+1] > water_level)
    delete_island(map, x+1, y, water_level);
  if(y > 0 && map[y-1][x] > water_level)
    delete_island(map, x, y-1, water_level);
  if(y < (MAP_SIZE-1) && map[y+1][x] > water_level)
    delete_island(map, x, y+1, water_level);
}

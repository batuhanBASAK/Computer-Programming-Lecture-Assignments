/* Efficiency of Code: 
 *
 *
 * In code there is a while loop that calls a function for each iteration
 * and that function has a for loop also. Therefore we can see there is a 
 * nested loop in program.
 *      Assume that each iteration of second loop last 1second and it iterates n times.
 * Then for one iteration of while loop lasts ( n x 1sec ) = n seconds. And n represents
 * how many different numbers exist on file. And while loop will been iterated n times also.
 * Therefore program lasts aproximately ( n x nsec) = n^2 seconds.
 *
 *
 */
#include <stdio.h>

#define MAX_NUMS 100

#define INPUTFILE "input.txt"
#define OUTPUTFILE "output.txt"

/* function prototypes */

int get_min_count(int nums[], int counts[], int size);
int get_index_num(int nums[], int target, int size);
void order_in_ascending(int nums[], int counts[], int to_nums[], int to_counts[], int size);
void read_input(FILE *fp, int nums[], int counts[], int *size);
void write_output(FILE *fp, int nums[], int counts[], int size);

int main(int argc, char const *argv[])
{
    FILE *fp;
    
    /* nums array to hold different numbers from input file */
    int nums[MAX_NUMS];
    /* counts array to hold count of numbers in array nums */
    int counts[MAX_NUMS];

    /* ordered version of arrays nums and counts */
    int ordered_nums[MAX_NUMS];
    int ordered_counts[MAX_NUMS];

    /* size of arrays nums and counts */
    int size;

    /* read input and get numbers, count of numbers and size of arrays */
    read_input(fp, nums, counts, &size);

    /* order numbers and its' counts in ascending order and 
     * store them in array ordered_nums and ordered_counts */
    order_in_ascending(nums, counts, ordered_nums, ordered_counts, size);

    /* write numbers and their counts to output file */
    write_output(fp, ordered_nums, ordered_counts, size);

    return 0;
}



/* gets next least number respect to counts, and not -1,
 * if counts are same return index of least number in array nums
 *
 * postcondition: proceed numbers' count has to be set by -1 to reuse
 *                this function
 *
 * This function is a helper function for function order_in_ascending
 *
 * returns index of least number
 */
int get_min_count(int nums[], int counts[], int size)
{
    int index_of_least;
    int min_count;
    int i;

    /* initialize min_count is the first count is not -1 */
    i = 0;
    while((i < size) && (counts[i] == -1))
        i++;
    min_count = counts[i];

    /* find current minimum count in counts */
    for(i=0; i < size; i++)
    {
        if(counts[i] < min_count && counts[i] != -1)
            min_count = counts[i];
    }


    /* initialize index_of_least to first number in nums
     * that has min_count counts
     */
    i = 0;
    while((i < size) && counts[i] != min_count)
        i++;
    index_of_least = i;

    /* find least number in nums has min_count counts */
    for(i=0; i < size; i++)
    {
        if((counts[i] == min_count) && (nums[i] < nums[index_of_least]))
            index_of_least = i;
    }

    return index_of_least;
}


/* gets index of target number in array nums
 * returns index of number in array
 * if target doesn't exist on array function returns -1
 */
int get_index_num(int nums[], int target, int size)
{
    int target_index;
    int i;

    target_index = -1;
    for(i=0; i < size; i++)
    {
        if(nums[i] == target)
        {
            target_index = i;
            break;
        }
    }

    return (target_index);
}


void order_in_ascending(int nums[], int counts[], int to_nums[], int to_counts[], int size)
{
    int index_of_least;
    int i;

    for(i=0; i < size; i++)
    {
        /* find current least number in number array */
        index_of_least = get_min_count(nums, counts, size);

        /* store current least number ans its count to 
         * arrays nums and counts respectively
         */
        to_nums[i] = nums[index_of_least];
        to_counts[i] = counts[index_of_least];

        /* set count of current least number as 0 
         * to reuse function find_least_num 
         */
        counts[index_of_least] = -1;
    }
}


/* read different numbers, computes their counts and size
 * of arrays nums and counts from file
 * postcondition: size can at most 100
 */
void read_input(FILE *fp, int nums[], int counts[], int *size)
{
    int i;
    int number; /* number read from file */
    int status; /* status of input file */
    int index;  /* index of number in array */


    *size = 0;  /* initialize size 0 */

    /* open input file */
    if ((fp = fopen(INPUTFILE, "r")) == NULL)
    {
        printf("Error: Opening file '%s'!\n", INPUTFILE);
    }
    else
    {
        /* If file opened correctly then keep proceeding */


        while((status = fscanf(fp, "%d", &number)) == 1)
        {
            /* a number could been read from file successfuly */

            /* find index of number in array */
            index = get_index_num(nums, number, *size);

            if(index == -1)
            {
                /* read number does not exist on array yet */

                /* put number and its count to arrays */
                nums[*size] = number;
                counts[*size] = 1;

                /* increase size by one */
                *size += 1;
            }
            else
            {
                /* read number exists on array */

                /* increase count of number by one */
                counts[index]++;
            }
        }

        /* close input file */
        fclose(fp);
    }
}


/* write numbers and their counts to output file */
void write_output(FILE *fp, int nums[], int counts[], int size)
{
    fp = fopen(OUTPUTFILE, "w");
    int i;

    for(i=0; i < size; i++)
    {
        fprintf(fp, "%d:%d\n", nums[i], counts[i]);
    }

    fclose(fp);
}
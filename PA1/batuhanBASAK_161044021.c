/* Homework 1 for CSE 102
 * Respect to the algorithm:
 * Each iterations main function gets a number from file
 * and sends number, numbers location on file and file pointer to
 * function called check_number_exists which loop all numbers interval
 * between begining of file -SEEK_SET- to location of number.
 *      If we assume that computaion lasts 1s for first number,
 * then computation lasts 100s for 10th number.
 *      Like nested loops, program will run O(n^2) time complexity.
 */


#include <stdio.h>

#define FILENAME "file.txt"

/* booleans to improve readability */
#define TRUE 1
#define FALSE 0

int check_number_exists (FILE* file_pointer, int target, int target_location);
int get_number_length (int number);

int main (int argc, const char **argv)
{
    FILE* file_pointer;     /* file pointer to hold file            */
    int status;             /* status of file_pointer               */
    int current_number;     /* current number read from file        */

    /* Holds information that number is repeated or not             */
    int repeated;           
    int number_length;      /* length of number                     */
    int location;           /* location of file pointer             */

    /* open file */
    file_pointer = fopen (FILENAME, "r+");

    /* loop file while an int could read, while status is 1, from file  */
    do
    {
        /* read an int from file_pointer into current_number            */ 
        status = fscanf (file_pointer, "%d", &current_number);

        /* set location                                                 */
        location = ftell(file_pointer);

        if (status == 1)
        {
            /* Proceed, if int number read successfuly from file        */


            /* Check current number is repeated number or not.
             * if repeated is TRUE, number repeated before,
             * else, number is not repeated.                            */
            repeated = check_number_exists (file_pointer,
                                            current_number,
                                            location);

            if (repeated == TRUE)
            {
                /* current number is repeated                           */

                /* put white spaces to location of current number       */
                number_length = get_number_length(current_number);
                
                /* go to beggining of current number position           */
                fseek (file_pointer, -1 * number_length, SEEK_CUR);
 
                /* put white spaces length of current number times
                 * to place of current number location, to delete
                 * current number.                                      */
                int index; /* to itarate loop                           */
                for (index=0; index < number_length; index++)
                    fputc (' ', file_pointer); 
            }

        }
    }while (status == 1);

    fclose(file_pointer);   /* close file */

    return 0;
}


/* Checks target number is exists before
 * @param file_pointer is file pointer holds file
 * @param target is int number want to check
 * @param target_location is int number holds end location of interval
 * @returns TRUE if target repeated in searched interval, else FALSE.
 */
int check_number_exists (FILE* file_pointer, int target, int target_location)
{
    int ans;        /* will been returned as answer                 */
    int index;      /* iterator                                     */
    int number;     /* temp number to hold read number from file    */
    int status;     /* status of file pointer                       */

    /* Go to beggining of the file */
    fseek(file_pointer, 0, SEEK_SET);

    /* set ans FALSE by default */
    ans = FALSE;

    /* set iterators location to begining of file                               */
    index = ftell(file_pointer);

    /* check interval by loop                                                   */
    while (index != target_location)
    {
        status = fscanf (file_pointer, "%d", &number);
        index = ftell(file_pointer); /* get current location of file pointer    */

        if (status == 1 && index != target_location)
        {
            /* If an int number read successfuly from file and 
             * read number is not real number, not at same location,
             * Then proceed                                                     */


            /* If read number is equal to target
             * target is a repeated number.
             * set ans TRUE, else keep proceed loop                             */
            if (number == target)
                ans = TRUE;
        }
    }

    /* Move back location where file pointer was before the function */
    fseek (file_pointer, target_location, SEEK_SET);


    /* return ans */
    return (ans); 
}



/* Gets the length of number
 * @param number is number to get length of it
 * @returns length of number as int val
 */
int get_number_length (int number)
{
    int length;         /* length of number                         */
    
    length = 1;         /* initialize length as 1                   */

    if (number < 0)
    {
        /* If number is negative                                    */

        length++;       /* increase length by one for '-' sign      */
        number *= -1;   /* make number positive for next steps      */    
    }

    while (number/10 != 0)
    {
        /* Loop while number is not 0                               */

        length++;    /* increase length for each iteration by one   */
        number /= 10;   /* divide number by 10                      */
    }

    return (length);
        
}

 

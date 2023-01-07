#include <stdio.h>
#include <stdlib.h> /* for function exit */
#include <math.h>

#define VALUES_ARRAY_SIZE 100
#define POLYNOMAIL_ARRAY_SIZE 1000


/* function prototypes */
void read_values(FILE *file_values, double values[], int values_max_size, int *number_of_values);
void read_polynomial(FILE *file_polynomial, char polynomial[], int size);
double compute_polynomial_value(char polynomial[], double value);
int is_digit(char c);
int len_int(int num);

int main(int argc, char const *argv[])
{
    /* values array holds values */
    double values[VALUES_ARRAY_SIZE] = {0};
    int number_of_values = 0; /* number of values in array values */

    /* polynomial in character array */
    char polynomial[POLYNOMAIL_ARRAY_SIZE] = {'\0'};

    /* file descriptors */
    FILE *input_file;
    FILE *output_file;

    /* computed value respect to a value from values array and polynomial */
    double computed_value;

    int i;

    /* read all values from values.txt file and store values in values array */
    read_values(input_file, values, VALUES_ARRAY_SIZE, &number_of_values);

    /* read polnomial in file polynomial.txt and store it in polynomial */
    read_polynomial(input_file, polynomial, POLYNOMAIL_ARRAY_SIZE);

    /** Open evaluation.txt file 
     * Iterate each value of values array and compute the
     * value respect to polynomial.txt. Store the computed value in
     * variable computed_value. And write computed_value into file
     * evaluation.txt
     */
    output_file = fopen("evaluations.txt","w");

    if(output_file == NULL)
    {
        /** if polynomial.txt could not been opened,
         * Then exit the code by EXIT_FAILURE status code
         */
        printf("Error: Opening file evaluations.txt!\n");
        fflush(stdout); /* make sure to print error message before exiting */
        exit(EXIT_FAILURE);
    }

    for(i=0; i < number_of_values; i++) /* iterate each value of values */
    {
        /* compute value */
        computed_value = compute_polynomial_value(polynomial, values[i]);

        /* write value into file evaluations.txt */
        fprintf(output_file, "%.2lf\n", computed_value);
    }

    /* close file evaluations.txt */
    fclose(output_file);

    return 0;
}


/**
 * read each value of file values.txt and store them in array
 * values. Also count number of values that values has. Stores
 * number of value data in number_of_values
 * 
 * 
 * file_values is file descriptor for file values.txt
 * values is array of double that will hold each value from file values.txt
 * values_max_size is maximum size of array values
 * number_of_values is number of values that array values has
 */
void read_values(FILE *file_values, double values[], int values_max_size, int *number_of_values)
{
    double value;   /* double value needed for iteration */
    int status;     /* status of file */

    /* open file values.txt */
    file_values = fopen("values.txt", "r");
    if(file_values == NULL)
    {
        /** if values.txt could not been opened,
         * Then exit the code by EXIT_FAILURE status code
         */
        printf("Error: Opening file values.txt!\n");
        fflush(stdout); /* make sure to print error message before exiting */
        exit(EXIT_FAILURE);
    }

    /** read values from file while there is value hasn't read yet
     * and maximum array size of values has not been reached
     */

    *number_of_values = 0; /* initialize number_of_values 0 */
    while((status = fscanf(file_values, "%lf", &value)) == 1 && *number_of_values < values_max_size)
    {
        values[*number_of_values] = value;  /* store value read from file in values array */
        (*number_of_values)++;  /* increase number_of_values by 1 for each iteration */
    }

    /* close file values.txt */
    fclose(file_values);

}


/**
 * read polynomial in file polynomial.txt as char array and 
 *        stores it in char array polynomial
 * 
 * file_polynomial is file descriptor for polynomial.txt
 * polynomial is Char array to hold polynomial in char array form
 * size is size of array polynomial
 */
void read_polynomial(FILE *file_polynomial, char polynomial[], int size)
{
    char c;     /* iterator variable need for loop */
    int i;

    /* open file polynomial.txt */
    file_polynomial = fopen("polynomial.txt", "r");
    if(file_polynomial == NULL)
    {
        /** if polynomial.txt could not been opened,
         * Then exit the code by EXIT_FAILURE status code
         */
        printf("Error: Opening file polynomial.txt!\n");
        fflush(stdout); /* make sure to print error message before exiting */
        exit(EXIT_FAILURE);
    }

    /** read file polynomial.txt char by char for each iteration of while loop.
     * And for each iteration store read char c to char array polynomial.
     */

    /** Iterate loop file there is char hasn't read yet in file polynomial.txt
     * And size of char array polynomial has not reached yet
     */
    for(i=0;(c = fgetc(file_polynomial)) != EOF && i < size; i++)
    {
        polynomial[i] = c;
    }
    polynomial[i] = '\0';   /* put null char end of string */

    /* close file polynomial.txt */
    fclose(file_polynomial);
}


/**
 * computes value respect to polynomial and returns result as double number
 * 
 * polynomial Char array holds polynomial
 * value Double value which is domian of polynomial function
 * returns double computed value respect to domain value and polynomial
 */
double compute_polynomial_value(char polynomial[], double value)
{
    double computed_value = 0.0;

    double coeff;   /* coefficient value */
    int exp;        /* exponential value  */

    int i; /* iterator which is need for loop */

    i = 0;
    while(polynomial[i] != '\0')
    {
        /* read coeff if exists */
        if(polynomial[i] == '-' && is_digit(polynomial[i+1]))
        {
            /* Coefficient is a negative number */
            sscanf(&polynomial[i], "%lf", &coeff);
        }
        else if(is_digit(polynomial[i]))
        {
            /* Coefficient is a positive number */
            sscanf(&polynomial[i], "%lf", &coeff);
        }
        else if(polynomial[i] == '-' && !is_digit(polynomial[i+1]))
        {
            /* Coefficient is -1 */
            coeff = -1;
        }
        else if(polynomial[i] == '+' && is_digit(polynomial[i+1]))
        {
            /* Coeff is a posivite number */
            i++;
            sscanf(&polynomial[i], "%lf", &coeff);
        }
        else
        {
            /* Coefficient is 1 */
            coeff = 1;
        }

        /* iterate i to the forward of x */
        while(polynomial[i] != 'x')
            i++;
        i++;

        /** check exponential char exists
         * If exponential char exists, then read exponent
         * else set exponent as 1 and continue
         */
        if(polynomial[i] == '^')
        {
            i++;
            sscanf(&polynomial[i], "%d", &exp);
            i += len_int(exp);
        }
        else
        {
            /* exponent is 1 */
            exp = 1;
        }

        /* calculate current monomial and add it to sum */
        computed_value += (coeff * pow(value, (double)exp));
    }

    return computed_value;
}


/**
 * takes a char c as argument and checks char c is digit or not
 * 
 * c is char to analyze
 * If char c is digit return 1, else 0
 */
int is_digit(char c)
{
    return ((c >= '0' && c <= '9') ? 1 : 0);
}


/**
 * Finds length of number and returns it
 * 
 * num is number
 * returns length of num
 */
int len_int(int num)
{
    int len; /* length of num */

    len = 1;

    if(num < 0)
    {
        /** If num is a negative number, then make it positive to proceed
         * and increase len by 1 because of '-' sign
         */
        num *= -1;
        len++;
    }

    /* count num */
    while((num/10) != 0)
    {
        num /= 10;
        len++;
    }

    return len;
}
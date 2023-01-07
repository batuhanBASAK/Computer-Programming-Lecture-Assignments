#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUTFILE "input.txt"
#define OUTPUTFILE "output.txt"
#define LINESIZE 250

int read_line(FILE *input_file, char line[], int line_size);
void write_node (FILE *output_file, char c, int inner_tree);
void write_tree(FILE *output_file, char line[], int inner_tree);

int main(int argc, const char *argv[])
{
    FILE *filedes;
    char line[LINESIZE] = {'\0'};
    int file_status;

    file_status = read_line(filedes, line, LINESIZE);
    if(file_status == 0)
    {
        printf("Error: Reading File '%s'!\n", INPUTFILE);
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    filedes = NULL;
    write_tree(filedes, line, 0);

    return 0;
}


/* reads a line that has a line_size size from input file 
 * If line is read successfuly from input file returns 1, else 0
 */
int read_line(FILE *input_file, char line[], int line_size)
{
    input_file = fopen(INPUTFILE, "r");
    if(input_file == NULL)
        return 0;

    fgets(line, line_size - 1, input_file);
    line[strlen(line) - 1] = '\0';

    fclose(input_file);

    return 1;
}


/* Writes current node c to output file with drawing inner tree
 * function prints inner_tree times char '-' then writes node char c
 * function runs recursively
 */
void write_node (FILE *output_file,char c, int inner_tree)
{
    if(inner_tree == 0)
    {
        fprintf(output_file, "%c\n", c);
    }
    else if(inner_tree > 0)
    {
        fprintf(output_file, "%c", '-');
        write_node(output_file, c, inner_tree-1);
    }
}


/* Converts line to tree by parsing string line char by char.
 * And analyes current char in line and proceeds to write tree
 * to file output file recursively by using helper function write_node, Which
 * writes each node to output file also recursively.
 */
void write_tree(FILE *output_file, char line[], int inner_tree)
{
    if(output_file == NULL)
    {
        /* output file hasn't opened yet. */

        /*  open output file */
        output_file = fopen(OUTPUTFILE, "w");
        if(output_file == NULL)
            return ;

    }


    if(line[0] == '\0')
    {
        /*  End of line has been reached. */
        /*  Close output file and end the process. */
        fclose(output_file);
    }
    else
    {
        /* Main case */

        if(line[0] == '(')
        {
            /* There is another inner tree there.
             * Increate inner_tree by 1.
             * And move to next char by recursion
             */
            inner_tree++;

            write_tree(output_file, &line[1], inner_tree);
        }

        else if(line[0] == ')')
        {
            /* An inner tree is closed.
             * Descrease inner_tree by 1.
             * Move to next char in string line by calling
             * function write_tree() recursively.
             */
            inner_tree--;

            /* call function for next iteration */
            write_tree(output_file, &line[1], inner_tree);
        }

        else if(line[0] == ',')
        {
            /* If current char first index of string line is ',',
             * move to next char. by calling write_tree function
             * recursively.
             */
            write_tree(output_file, &line[1], inner_tree);
        }
        else
        {
            /* Current char is current node.
             * write current node to output file.
             * Then move to next char line[1] by
             * calling function write_tree recursively.
             */

            /*  write node to output file */
            write_node(output_file, line[0], inner_tree);

            /* move next char to proceed */
            write_tree(output_file, &line[1], inner_tree);
        }
    }
}



#include <stdio.h>
#include <string.h> /* for function strlen() */
#include <stdlib.h> /* for function exit() */


/* boolean types to improve readabilty */
#define TRUE 1
#define FALSE 0

#define LINESIZE 100


/* input types for input used in function compute_cost */
#define COST_TYPE 1
#define OBJ_TYPE 2
#define SUB_COMPONENT_TYPE 3


/* function prototypes */
void get_line(char line[], int size);
int is_alpha(char c);
int is_cost_type(char input[]);
int is_obj_type(char input[]);
int find_input_type(char input[]);
int find_index_of_char(char input[], char target);
int count_component_count(char input[]);
double compute_cost(char input[]);



int main(int argc, const char *argv[])
{
  char obj[LINESIZE] = {'\0'};
  double cost;

  printf("Define object?\n");
  get_line(obj, LINESIZE);

  cost = compute_cost(obj);

  printf("Total cost of %s is %.1lf\n", obj, cost);

  return 0;
}


/* get a line of input from user and stores it in char array line[]
 * size is the maximum size of line[]
 */
void get_line(char line[], int size)
{
  fgets(line, size-1, stdin);
  line[strlen(line) - 1] = '\0';
}


/* takes a char c as an argument and
 * returns TRUE if c is an alpha char,
 * else FALSE
 */
int is_alpha(char c)
{
  return (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) ? TRUE : FALSE);
}


/* Takes a char array input[] and
 * returns TRUE if input array is COST_TYPE,
 * else FALSE 
 */
int is_cost_type(char input[])
{
  int i;
  int ans;

  for(i=0, ans=TRUE; input[i] != '\0' && i < LINESIZE; i++)
    if(is_alpha(input[i]))
    {
      ans = FALSE;
      break;
    }

  return ans;
}


/* Takes a char array input[] and
 * returns TRUE if input array is OBJ_TYPE,
 * else FALSE 
 */
int is_obj_type(char input[])
{
  int i;
  int ans;

  for(i=0, ans=TRUE; input[i] != '\0' && i < LINESIZE; i++)
    if(input[i] == '*')
    {
      ans = FALSE;
      break;
    }

  return ans;
}


/* takes input as char array and returns its
 * input type
 *
 *
 * INPUT TYPES:
 * ============
 * 
 *   If input is a cost of object is called COST_TYPE,
 *
 *   If input is the name of object is caled OBJ_TYPE,
 *
 *   If input is subcomponents of object, such as '2*Wheel + 1 * Frame',
 *   is called SUB_COMPONENT_TYPE
 *   
 */
int find_input_type(char input[])
{
  int ans;

  if(is_cost_type(input))
    ans = COST_TYPE;  
  else if(is_obj_type(input))
    ans = OBJ_TYPE;
  else
    ans = SUB_COMPONENT_TYPE;

  return ans;
}


/* Takes an array of char and a target char
 * and returns the index of target char in array.
 * If there is no char target in array returns -1 as flag.
 *
 * input is char array stores input
 * target is target we are searching in array input
 */
int find_index_of_char(char input[], char target)
{
  int i;
  int ans;

  for(i=0, ans=-1; input[i] != '\0' && i < LINESIZE; i++)
    if(input[i] == target)
    {
      ans = i;
      break;
    }

  return ans;
}


/* Takes input as char array and
 * returns how many component exists in input.
 * 
 * For '2*Wheel + 1*Frame'
 *   'wheel' and 'Frame' are components and 
 *   for this example the component count is 2
 */
int count_component_count(char input[])
{
  int i;
  int count;

  for(i=0, count=1; i < LINESIZE && input[i] != '\0'; i++)
    if(input[i] == '+')
      count++;

  return count;
}


/* Takes input as char array, computes and returns cost of given input
 * Input could be:
 *   (1) A cost value, also called COST_TYPE, 
 *   (2) Name of object, also called OBJ_TYPE,
 *   (3) Component of object, also called SUB_COMPONENT_TYPE.

 * function determines which input type input is and keep proceeds 
 * respect to input type.
 *
 * RESPECT TO THE HOMEWORK function compute_cost RUNS RECURSIVELY!
 *
 * returns cost of object as double value.
 */
double compute_cost(char input[])
{
  int component_count;
  char component_name[LINESIZE] = {'\0'};
  char new_input[LINESIZE] = {'\0'};
  int input_type;
  int sub_component_count;
  double cost;
  int index;

  /* determine input type of input */
  input_type = find_input_type(input); 

  /* proceed respect to input type */
  switch(input_type)
  {
    case COST_TYPE:
      /* Input type is a cost value */

      /* read cost from input value as double value */
      sscanf(input, "%lf", &cost);

      break;
    case OBJ_TYPE:
      /* Input is name of the object */

      /* assume object name as a component name
       * read component name from input
       */
      sscanf(input, "%s", component_name);
      
      /* Ask more information about component */
      printf("What is %s?:\n", component_name);
      get_line(new_input, LINESIZE); /* get a new line as an input from user */

      /* respect to given new input compute cost */
      cost = compute_cost(new_input);

      break;
    case SUB_COMPONENT_TYPE:
      /* Input is subcomponent(s) for current component. */


      /* read first occured component name and its count from input */
      sscanf(input, " %d * %[^'+'' ']s", &component_count, component_name);

      /* get count of sub_component(s) in input */
      sub_component_count = count_component_count(input);

      if(sub_component_count == 1)
      {
        /* There is only a component in input */ 

        /* compute given component
         * and multiply it by count of it to compute cost
         */
        cost = (component_count * compute_cost(component_name));
      }
      else
      {
        /* There are more than a subcomponent in input */

        /* find index of next occured component,
         * where index of first occured char '+' in array 
         */
        index = find_index_of_char(input, '+');


        /* compute component count time component first.
         * Then add the cost of current component to next component recursively
         */
        cost = (component_count * compute_cost(component_name))
               + compute_cost(&input[++index]); /* send input start where
                                                 * after first occured '+' sign
                                                 */
      }
       
      break;
    default:
      /* Default case to make sure everthing works well */

      /* If input type is invalid program is stopped executing
       * by exit status code EXIT_FAILURE 
       * and prints an error message to the standard output.
       */
      printf("Invalid input!\n");
      fflush(stdout);
      exit(EXIT_FAILURE);
  }

  return cost; /* finally returns cost */
}

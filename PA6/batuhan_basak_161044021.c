#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CIRCUITFILE "circuit.txt"
#define INPUTFILE "input.txt"

#define STR_SIZE 10

typedef enum
{
  INPUT,
  AND,
  OR,
  NOT,
  FLIPFLOP
}gate_t;


typedef enum {FALSE, TRUE}bool_t;


typedef struct
{
  char **input_names;
  bool_t *input_values;
  unsigned int input_count;
}circuit_t;


/* function prototypes */

void init_circuit(circuit_t *circuit);
void add_input(circuit_t *circuit, char *name, const bool_t val);
void free_circuit(circuit_t *circuit);

gate_t get_gate_type(char *str);

bool_t get_value(circuit_t c, const char *target);
bool_t and_operation(bool_t bool1, bool_t bool2);
bool_t or_operation(bool_t bool1, bool_t bool2);
bool_t not_operation(bool_t bool);
bool_t flipflop_operation(bool_t bool, bool_t former_out);


int main(int argc, const char *argv[])
{
  /* file descriptors */
  FILE *circuit_file;
  FILE *input_file;

  /* circuit */
  circuit_t circuit;

  /* strings to hold datas */
  char temp[STR_SIZE] = {'\0'};
  char gate_type[STR_SIZE] = {'\0'};

  /* strings to hold operands */
  char operand1[STR_SIZE] = {'\0'};
  char operand2[STR_SIZE] = {'\0'};

  /* for iteration in files */
  int c;

  bool_t former_out = FALSE;

  bool_t value;
  gate_t gate;

  int status; /* file status */

  /* open input file */
  input_file = fopen(INPUTFILE, "r");
  if(input_file == NULL)
  {
    printf("Error: Opening File '%s'!\n", INPUTFILE);
    fflush(stdout);
    exit(EXIT_FAILURE);
  }

  /* main loop loops while there is input field left in
   * file input.txt to proceed
   */
  while((status = fscanf(input_file, "%d", &c)) == 1)
  {
    /* move a char back in file to proceed */
    fseek(input_file, -1, SEEK_CUR);

    /* init circuit */
    init_circuit(&circuit);

    /* open circuit file */
    circuit_file = fopen(CIRCUITFILE, "r");
    if(circuit_file == NULL)
    {
      printf("Error: Opening File '%s'!\n", CIRCUITFILE);
      fflush(stdout);
      exit(EXIT_FAILURE);
    }

    /* read each operation line by line and compute last operation */
    while((status = fscanf(circuit_file, "%s", gate_type)) == 1)
    {
      /* find gate in gate_t from string gate_type */
      gate = get_gate_type(gate_type);
      switch(gate)
      {
        /* proceed respect to type of gate */

        case INPUT:
          /* gate type is INPUT */

          /* read input names from file circuit.txt and
           * input values from file input.txt while there is
           * in current line.
           */
          while((c = fgetc(circuit_file)) != '\n')
          {
            fseek(circuit_file, -1, SEEK_CUR);

            fscanf(circuit_file, "%s", temp);
            fscanf(input_file, "%d", &value);

            /* add input to circuit */
            add_input(&circuit, temp, value);
          }
          break;
        case AND:
          /* gate type is AND */

          /* read new input name and operands names */
          fscanf(circuit_file, "%s %s %s", temp, operand1, operand2);

          /* calculate value */
          value = and_operation(get_value(circuit, operand1),
              get_value(circuit, operand2));

          /* add input to circuit */
          add_input(&circuit, temp, value);

          break;
        case OR:
          /* gate type is OR */


          /* read new input name and operands names */
          fscanf(circuit_file, "%s %s %s", temp, operand1, operand2);


          /* calculate value */
          value = or_operation(get_value(circuit, operand1),
            get_value(circuit, operand2));

          /* add input to circuit */
          add_input(&circuit, temp, value);

          break;
        case NOT:
          /* gate type is NOT */

          /* read new input name and operand */
          fscanf(circuit_file, "%s %s", temp, operand1);

          /* get value */
          value = not_operation(get_value(circuit, operand1));
          
          /* add input to circuit */
          add_input(&circuit, temp, value);
          
          break;
        case FLIPFLOP:
          /* gate type is FLIPFLOP */

          /* read new input name and operand */
          fscanf(circuit_file, "%s %s", temp, operand1);

          /* get value */
          value = flipflop_operation(get_value(circuit, operand1), former_out);

          /* set new former out */
          former_out = value;

          /* add input to circuit */
          add_input(&circuit, temp, value);

          break;
      }
    }
    /* close file circuit.txt */
    fclose(circuit_file);

    /* print result to standard output */
    printf("%d\n", circuit.input_values[circuit.input_count - 1]);

    /* deallocate memory of circuit holds to avoid memory leak */
    free_circuit(&circuit);
  }

  /* close file input.txt */
  fclose(input_file);


  return 0;
}


/* initializes the circuit */
void init_circuit(circuit_t *circuit)
{
  circuit->input_names = NULL;
  circuit->input_values = NULL;
  circuit->input_count = 0;
}


/* add new input which has name name and value val into circuit circuit */
void add_input(circuit_t *circuit, char *name, const bool_t val)
{
  if(circuit->input_count == 0)
  {
    /* circuit has no input yet */

    circuit->input_names = (char**)malloc(sizeof(char*));
    circuit->input_names[0] = (char*)malloc(sizeof(char)*STR_SIZE);
    strcpy(circuit->input_names[0], name);

    circuit->input_values = (bool_t*)malloc(sizeof(bool_t));
    circuit->input_values[0] = val;
  }
  else
  {
    circuit->input_names = (char**)realloc(circuit->input_names,
      sizeof(char*) * (circuit->input_count + 1));
    circuit->input_names[circuit->input_count] =
      (char*)malloc(sizeof(char)*STR_SIZE);
    strcpy(circuit->input_names[circuit->input_count], name);

    circuit->input_values = (bool_t*)realloc(circuit->input_values,
      sizeof(bool_t) * (circuit->input_count + 1));
    circuit->input_values[circuit->input_count] = val;
  }
  circuit->input_count += 1;
}


/* deallocates memory of given circuit as parameter */
void free_circuit(circuit_t *circuit)
{
  free(circuit->input_names);
  free(circuit->input_values);
  circuit->input_count = 0;
}


/* returns gate type in gate_t respect to given string as parameter */
gate_t get_gate_type(char *str)
{
  gate_t ans;
  if(strcmp(str, "INPUT") == 0)
      ans = INPUT;
  else if(strcmp(str, "AND") == 0)
    ans = AND;
  else if(strcmp(str, "OR") == 0)
    ans = OR;
  else if(strcmp(str, "NOT") == 0)
    ans = NOT;
  else if(strcmp(str, "FLIPFLOP") == 0)
    ans = FLIPFLOP;
  return ans;
}


/* returns value of target input in circuit c */
bool_t get_value(circuit_t c, const char *target)
{
  int i;
  bool_t ans;

  for(i=0; i < c.input_count; i++)
  {
    if(strcmp(c.input_names[i], target) == 0)
    {
      ans = c.input_values[i];
      break;
    }
  }

  return ans;
}


/* computes operation for (bool1 and bool2) */
bool_t and_operation(bool_t bool1, bool_t bool2)
{
  return ((bool1 || bool2) ? TRUE : FALSE);
}


/* computes operation for (bool1 or bool2) */
bool_t or_operation(bool_t bool1, bool_t bool2)
{
  return ((bool1 && bool2) ? TRUE : FALSE);
}


/* computes not operation for bool */
bool_t not_operation(bool_t bool)
{
  return (!bool);
}


/* computes fliplop operation for bool */
bool_t flipflop_operation(bool_t bool, bool_t former_out)
{
  bool_t ans;
  if(bool == FALSE && former_out == FALSE)
    ans = FALSE;
  if(bool == FALSE && former_out == TRUE)
    ans = TRUE;
  if(bool == TRUE && former_out == FALSE)
    ans = TRUE;
  if(bool == TRUE && former_out == TRUE)
    ans = FALSE;

  return ans;
}


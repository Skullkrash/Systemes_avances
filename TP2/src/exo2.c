#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/wait.h>

#define STDOUT 1
#define STDERR 2

#define MAX_PATH_LENGTH 4096

#define USAGE_SYNTAX "[OPTIONS] -i INPUT -o OUTPUT"
#define USAGE_PARAMS "OPTIONS:\n\
  -i, --input  INPUT_FILE  : input file\n\
  -o, --output OUTPUT_FILE : output file\n\
***\n\
  -v, --verbose : enable *verbose* mode\n\
  -h, --help    : display this help\n\
"

// Display binary usage by printing on stdout all available options
void print_usage(char* bin_name)
{
  dprintf(1, "USAGE: %s %s\n\n%s\n", bin_name, USAGE_SYNTAX, USAGE_PARAMS);
}


// Check if variable must be freed
void free_if_needed(void* to_free)
{
  if (to_free != NULL) free(to_free);  
}


// Create a duplicate of the optarg global string
char* dup_optarg_str()
{
  char* str = NULL;

  if (optarg != NULL)
  {
    str = strndup(optarg, MAX_PATH_LENGTH);
    
    // Checking if ERRNO is set
    if (str == NULL) 
      perror(strerror(errno));
  }

  return str;
}


// Binary options declaration (must end with {0,0,0,0})
static struct option binary_opts[] = 
{
  { "help",    no_argument,       0, 'h' },
  { "verbose", no_argument,       0, 'v' },
  { "input",   required_argument, 0, 'i' },
  { "output",  required_argument, 0, 'o' },
  { 0,         0,                 0,  0  } 
};

// Binary options string (linked to option declaration)
const char* binary_optstr = "hvi:o:";


int main(int argc, char** argv)
{
  // Parsing options
  int opt = -1;
  int opt_idx = -1;

  while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
  {
    switch (opt)
    {
      case 'h':
        print_usage(argv[0]); 
        exit(EXIT_SUCCESS);
      default :
        break;
    }
  } 

  if (argv[1] != NULL)
  {
    printf("%s\n", argv[1]);
  } else {
    printf("No argument\n");
    exit(EXIT_FAILURE);
  }
  
  int p2, file_descriptor;

  if ((p2 = fork()) == 0) { // Child process
    printf("-----CHILD-----\n");
    printf("Child PID : %d\n", getpid());
    close(1);
    char* buf = "/tmp/proc-exerciseXXXXXX";
    file_descriptor = mkstemp(buf);
    if (file_descriptor == -1)
    {
      perror("mkstemp");
      exit(EXIT_FAILURE);
    }
    

    int ret = dup2(file_descriptor, 1);
    if (ret == -1)
    {
      perror("dup2");
      exit(EXIT_FAILURE);
    }
    
    printf("ret : %d\n", ret);

    // printf("file_descriptor : %d\n", dup2(file_descriptor, 1));
    printf("Child success !\n");
    exit(EXIT_SUCCESS);
  }
  else { // Parent process
    printf("-----PARENT-----\n");
    printf("Parent PID : %d\n", getpid());
    printf("Waiting for child process to finish...\n");
    waitpid(p2, NULL, 0); // Wait for child process to finish
    printf("Parent finished waiting for child !\n");
    printf("Parent success !\n");
    exit(EXIT_SUCCESS);
  }


  close(file_descriptor);

  return EXIT_SUCCESS;
}
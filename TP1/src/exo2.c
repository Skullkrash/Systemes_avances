#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<getopt.h>

#define STDOUT 1
#define STDERR 2

#define MAX_PATH_LENGTH 4096

#define USAGE_SYNTAX "[OPTIONS] -i INPUT"
#define USAGE_PARAMS "OPTIONS:\n\
  -i, --input  INPUT_FILE  : input file\n\
***\n\
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
  { "input",   required_argument, 0, 'i' },
  { 0,         0,                 0,  0  } 
};

// Binary options string (linked to option declaration)
const char* binary_optstr = "hvi:o:";


int main(int argc, char** argv)
{
  // Binary variables
  short int is_verbose_mode = 0;
  char* bin_input_param = NULL;

  // Parsing options
  int opt = -1;
  int opt_idx = -1;

  while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
  {
    switch (opt)
    {
      case 'i':
        //input param
        if (optarg)
        {
          bin_input_param = dup_optarg_str();         
        }
        break;
      case 'h':
        print_usage(argv[0]);

        free_if_needed(bin_input_param);
 
        exit(EXIT_SUCCESS);
      default :
        break;
    }
  } 

  // Checking binary requirements (parameters)
  if (bin_input_param == NULL)
  {
    dprintf(STDERR, "Bad usage! See HELP [--help|-h]\n");
    free_if_needed(bin_input_param);
    exit(EXIT_FAILURE);
  }
  
  // ---------- ACTUAL MAIN CODE (outputs input file content in reverse order)----------

  int f1, val;
  char buff;

  f1 = open(bin_input_param, O_RDONLY);

  if (f1 == -1 ) {
    perror(strerror(errno));
    exit(EXIT_FAILURE);
  }

  val = lseek(f1, 0, SEEK_END);

  if (val == -1) {
    perror(strerror(errno));
    exit(EXIT_FAILURE);
  }

  while (val-- > 0) {
    lseek(f1, val, SEEK_SET);
    read(f1, &buff, 1);
    write(STDOUT, &buff, 1);
  }

  // Closing descriptor
  if (close(f1) == -1) {
    perror(strerror(errno));
    exit(1);
  }

  // Freeing allocated data
  free_if_needed(bin_input_param);

  return EXIT_SUCCESS;
}
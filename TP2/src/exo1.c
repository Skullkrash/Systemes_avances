#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<getopt.h>
#include<unistd.h>
#include<sys/wait.h>

#define STDOUT 1
#define STDERR 2

#define MAX_PATH_LENGTH 4096

#define USAGE_SYNTAX "[OPTIONS]"
#define USAGE_PARAMS "OPTIONS:\n\
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

  // Checking binary requirements (parameters)
  if (argc > 1)
  {
    dprintf(STDERR, "Bad usage! See HELP [--help|-h]\n");

    exit(EXIT_FAILURE);
  }

  // ---------- ACTUAL MAIN CODE (printing parent / child process informations) ----------

  int f1;
  int status;

  if ((f1 = fork()) == 0) {
    printf("__CHILD__\n");
    int pid = getpid();

    printf("PID :%i\n", pid);
    printf("PPID :%i\n", getppid());

    exit(pid % 10);
  } else {
    
    waitpid(f1, &status, 0);
    printf("__PARENT__\n");
    printf("Child PID :%i\n", f1);
    
    if (WIFEXITED(status)) {
      printf("Child exited with status : %d\n", WEXITSTATUS(status));
    } else {
      printf("Child terminated abnormally\n");
    }
  }

  return EXIT_SUCCESS;
}
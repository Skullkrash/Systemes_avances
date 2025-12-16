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

  // ---------- ACTUAL MAIN CODE (read the pdf - exo 2) ----------
  // Nous ne sommes pas sur d'avoir complètement compris l'énoncé de cet exercice
  // Il se peut donc que notre code ne corresponde pas exactement à ce qui est attendu
  // Cependant, nous avons fait de notre mieux pour implémenter ce qui était demandé

  if (argv[1] != NULL && argv[2] == NULL) 
  {
    write(STDOUT, argv[1], strlen(argv[1]));
  } 
  else if (argv[1] == NULL) {
    perror("No argument provided");
    exit(EXIT_FAILURE);
  } 
  else if (argv[2] != NULL)
  {
    perror("Too many arguments provided");
    exit(EXIT_FAILURE); 
  }

  if (fork() == 0) {
    int pid = getpid();
    printf("Child PID : %d\n", pid);

    // close(STDOUT); // variante 1 
    close(STDERR);

    char buf[] = "/tmp/proc-exerciseXXXXXX";
    int fd = mkstemp(buf);

    if (fd == -1) {
      perror("mkstemp");
      exit(EXIT_FAILURE);
    } 

    // int new_fd = dup2(fd, STDOUT); // variante 1 
    int new_fd = dup2(fd, STDERR);
    if (new_fd == -1) {
      perror("dup2");
      exit(EXIT_FAILURE);
    }

    // dprintf(STDERR,"dup2 returned : %d\n", new_fd); // variante 1 
    dprintf(STDOUT,"dup2 returned : %d\n", new_fd);
    execvp(argv[1], &argv[1]);
    
    close(fd);
    perror("execvp failed");
    exit(EXIT_FAILURE);
  } else { // parent process
      printf("Parent PID : %d\n", getpid());
      waitpid(-1, NULL, 0);
      printf("That's all folks !\n");
  }


  return EXIT_SUCCESS;
}
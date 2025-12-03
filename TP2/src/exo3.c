#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<unistd.h>
#include<sys/wait.h>
#include<getopt.h>

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

  // ---------- ACTUAL MAIN CODE (does the equivalent of command : ps eaux | grep "^root " > /dev/null && echo "root est connecté") ----------
  
  int fd[2];
  int status, grep_status, ps_status;

  status = pipe(fd);

  if (status == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  // First son, executes 'ps eaux'
  if(fork() == 0) {

    // Redirects stdout to pipe write end
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
    
    ps_status = execlp("ps", "ps", "eaux", NULL);

    if (ps_status == -1) {
      perror("execlp ps");
      exit(EXIT_FAILURE);
    }

    exit(ps_status);
  } 
  
  // Second son, executes 'grep "^root"'
  if (fork() == 0) {
    // Redirects stdout to /dev/null
    int null_fd = open("/dev/null", O_WRONLY);
    if(null_fd == -1) {
      perror("open /dev/null");
      exit(EXIT_FAILURE);
    }
    dup2(null_fd, STDOUT);
    close(null_fd);

    // Executes grep with stdin redirected from pipe
    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    grep_status = execlp("grep", "grep", "^root", NULL);
    exit(grep_status);
  }

  close(fd[0]);
  close(fd[1]);
  
  // Waiting for both sons to finish
  wait(&ps_status);
  wait(&grep_status);

  // Check return code of grep and display appropriate message
  if(WEXITSTATUS(grep_status) != 0) {
    write(STDOUT, "root n'est pas connecté\n", 25);
    exit(EXIT_FAILURE);
  } else {
    // grep a trouvé root
    write(STDOUT, "root est connecté\n", 19);
  }

  return EXIT_SUCCESS;
}
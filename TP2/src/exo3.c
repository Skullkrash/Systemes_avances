/**
 * \file skeleton.c
 * \brief Basic parsing options skeleton.
 * \author Pierre L. <pierre1.leroy@orange.com>
 * \version 0.1
 * \date 10 septembre 2016
 *
 * Basic parsing options skeleton exemple c file.
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>

#include<getopt.h>


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

/**
 * Procedure which displays binary usage
 * by printing on stdout all available options
 *
 * \return void
 */
void print_usage(char* bin_name)
{
  dprintf(1, "USAGE: %s %s\n\n%s\n", bin_name, USAGE_SYNTAX, USAGE_PARAMS);
}


/**
 * Procedure checks if variable must be free
 * (check: ptr != NULL)
 *
 * \param void* to_free pointer to an allocated mem
 * \see man 3 free
 * \return void
 */
void free_if_needed(void* to_free)
{
  if (to_free != NULL) free(to_free);  
}


/**
 *
 * \see man 3 strndup
 * \see man 3 perror
 * \return
 */
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


/**
 * Binary options declaration
 * (must end with {0,0,0,0})
 *
 * \see man 3 getopt_long or getopt
 * \see struct option definition
 */
static struct option binary_opts[] = 
{
  { "help",    no_argument,       0, 'h' },
  { "verbose", no_argument,       0, 'v' },
  { "input",   required_argument, 0, 'i' },
  { "output",  required_argument, 0, 'o' },
  { 0,         0,                 0,  0  } 
};

/**
 * Binary options string
 * (linked to optionn declaration)
 *
 * \see man 3 getopt_long or getopt
 */ 
const char* binary_optstr = "hvi:o:";



/**
 * Binary main loop
 *
 * \return 1 if it exit successfully 
 */
int main(int argc, char** argv)
{
  /**
   * Binary variables
   * (could be defined in a structure)
   */
  // short int is_verbose_mode = 0;
  char* bin_input_param = NULL;
  char* bin_output_param = NULL;

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
      case 'o':
        //output param
        if (optarg)
        {
          bin_output_param = dup_optarg_str();
        }
        break;
      case 'v':
        //verbose mode
        // is_verbose_mode = 1;
        break;
      case 'h':
        print_usage(argv[0]);

        free_if_needed(bin_input_param);
        free_if_needed(bin_output_param);
 
        exit(EXIT_SUCCESS);
      default :
        break;
    }
  } 

  /**
   * Checking binary requirements
   * (could defined in a separate function)
  //  */
  // if (bin_input_param == NULL || bin_output_param == NULL)
  // {
  //   dprintf(STDERR, "Bad usage! See HELP [--help|-h]\n");

  //   // Freeing allocated data
  //   free_if_needed(bin_input_param);
  //   free_if_needed(bin_output_param);
  //   // Exiting with a failure ERROR CODE (== 1)
  //   exit(EXIT_FAILURE);
  // }

  // Business logic must be implemented at this point

  // ps eaux | grep "^root " > /dev/null && echo "root est connecté"
  
  int fd[2];
  int status, grep_status, ps_status;

  status = pipe(fd);

  if (status == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  // premier fils, execute ps eaux
  if(fork() == 0) {

    // rediriger la sortie standard vers le pipe
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
    
    ps_status = execlp("ps", "ps", "eaux", NULL);

    if (ps_status == -1) {
      perror("execlp ps");
      exit(EXIT_FAILURE);
    }

    exit(ps_status);
  } 
  
  // second fils, execute grep "^root"
  if (fork() == 0) {
    // rediriger la sortie standard vers /dev/null
    int null_fd = open("/dev/null", O_WRONLY);
    if(null_fd == -1) {
      perror("open /dev/null");
      exit(EXIT_FAILURE);
    }
    dup2(null_fd, STDOUT);
    close(null_fd);

    // execution de grep avec l'entrée standard redirigée depuis le pipe
    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    grep_status = execlp("grep", "grep", "^root", NULL);
    exit(grep_status);
  }

  close(fd[0]);
  close(fd[1]);
  
  // attendre les deux fils
  wait(&ps_status);
  wait(&grep_status);

  // vérifier le code de retour de grep et afficher le message approprié
  if(WEXITSTATUS(grep_status) != 0) {
    write(STDOUT, "root n'est pas connecté\n", 25);
    exit(EXIT_FAILURE);
  } else {
    // grep a trouvé root
    write(STDOUT, "root est connecté\n", 19);
  }

  // Freeing allocated data
  free_if_needed(bin_input_param);
  free_if_needed(bin_output_param);

  return EXIT_SUCCESS;
}
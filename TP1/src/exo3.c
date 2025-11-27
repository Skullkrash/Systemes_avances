/**
 * \file exo1.c
 * \brief Basic parsing options skeleton.
 * \author Pierre L. <pierre1.leroy@orange.com>
 * \version 0.1
 * \date 10 septembre 2016
 *
 * Basic parsing options skeleton exemple c file.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>


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


char* get_file_path(const char* dir_path, const char* file_name) {
    char* file_path = malloc((strlen(dir_path) + strlen(file_name) + 2) * sizeof(char));
    if (file_path == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    strcpy(file_path, dir_path);
    strcat(file_path, "/");
    strcat(file_path, file_name);
    return file_path;
}

void print_permissions(mode_t mode) {
    printf( (S_ISDIR(mode)) ? "d" : "-");
    printf( (mode & S_IRUSR) ? "r" : "-");
    printf( (mode & S_IWUSR) ? "w" : "-");
    printf( (mode & S_IXUSR) ? "x" : "-");
    printf( (mode & S_IRGRP) ? "r" : "-");
    printf( (mode & S_IWGRP) ? "w" : "-");
    printf( (mode & S_IXGRP) ? "x" : "-");
    printf( (mode & S_IROTH) ? "r" : "-");
    printf( (mode & S_IWOTH) ? "w" : "-");
    printf( (mode & S_IXOTH) ? "x" : "-");
}


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
    //   char* bin_output_param = NULL;

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
            // case 'o':
            //     //output param
            //     if (optarg)
            //     {
            //         bin_output_param = dup_optarg_str();
            //     }
            //     break;
            // case 'v':
            //     //verbose mode
            //     is_verbose_mode = 1;
            //     break;
            case 'h':
                print_usage(argv[0]);

                free_if_needed(bin_input_param);
                // free_if_needed(bin_output_param);
        
                exit(EXIT_SUCCESS);
            default :
                break;
        }
    } 

    /**
     * Checking binary requirements
     * (could defined in a separate function)
     */
    if (bin_input_param == NULL)
    {
        dprintf(STDERR, "Bad usage! See HELP [--help|-h]\n");

        // Freeing allocated data
        free_if_needed(bin_input_param);
        // free_if_needed(bin_output_param);
        // Exiting with a failure ERROR CODE (== 1)
        exit(EXIT_FAILURE);
    }


    // Printing params
    // dprintf(1, "** PARAMS **\n%-8s: %s\n%-8s: %d\n\n", 
    //     "input",   bin_input_param,
    //     "verbose", is_verbose_mode);

    DIR *dir = opendir(bin_input_param);
    if (dir == NULL) {
        perror(strerror(errno)); 
        return EXIT_FAILURE;
    }

    struct dirent *directory_reader;

    while (dir) {
        directory_reader = readdir(dir);
        if (!directory_reader) {
            break;
        }
        
        struct stat file_stat;
        char* file_path = get_file_path(bin_input_param, directory_reader->d_name);
        if (stat(file_path, &file_stat) == -1) {
            perror(strerror(errno));
            free(file_path);
            break;
        }
        print_permissions(file_stat.st_mode);
        struct passwd *pw = getpwuid(file_stat.st_uid);
        struct group  *gr = getgrgid(file_stat.st_gid);
        printf(" %s : ", (pw ? pw->pw_name : "unknown"));
        printf("%s ", (gr ? gr->gr_name : "unknown"));
        printf("%lld ", (long long)file_stat.st_size);
        char timebuf[80];
        struct tm *tm_info = localtime(&file_stat.st_mtime);
        strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", tm_info);
        printf("%s", timebuf);
        
        free(file_path);
        printf(" %s\n", directory_reader->d_name);
    }

    closedir(dir);
    
    // Freeing allocated data
    free_if_needed(bin_input_param);

    return EXIT_SUCCESS;
}
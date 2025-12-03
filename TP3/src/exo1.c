#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <getopt.h>
#include <unistd.h>
#include <pthread.h>

#define STDOUT 1
#define STDERR 2

#define MAX_PATH_LENGTH 4096

#define USAGE_SYNTAX "[OPTIONS] -i INPUT_INTEGER"
#define USAGE_PARAMS "OPTIONS:\n\
  -i, --input  INPUT_INTEGER  : number of threads to create\n\
***\n\
  -h, --help    : display this help\n\
"

#define SIZE (int)1e8
int hugeTab[SIZE];

// Mutex used to protect access to shared result variables (affectations / reading of thread_data structure)
static pthread_mutex_t results_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct thread_data {
  int* huge_table;
  int chunk_start;
  int chunk_end;
  int max;
  int min;
  int* time_for_max;
  int* time_for_min;
} thread_data_t;

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

// Function executed by each thread to find max and min in its assigned chunk
void* thread_function(void* arg) {
  thread_data_t* data = (thread_data_t*) arg;
  int* huge_table = data->huge_table;
  int chunk_start = data->chunk_start;
  int chunk_end = data->chunk_end;
  int* time_for_max = data->time_for_max;
  int* time_for_min = data->time_for_min;
  
  // If chunk is empty
  if (chunk_start >= chunk_end) {
    pthread_exit(NULL);
  }

  // Max and min to find in assigned chunk
  int local_max = huge_table[chunk_start];
  int local_min = huge_table[chunk_start];

  struct timeval tv_start, tv_now;
  gettimeofday(&tv_start, NULL);
  int end_time_min = 0, end_time_max = 0;

  for (int i = chunk_start; i < chunk_end; i++) {
    if (huge_table[i] > local_max) {
      local_max = huge_table[i];
      gettimeofday(&tv_now, NULL);
      end_time_max = (tv_now.tv_sec - tv_start.tv_sec) * 1000000L + (tv_now.tv_usec - tv_start.tv_usec);
    }
    if (huge_table[i] < local_min) {
      local_min = huge_table[i];
      gettimeofday(&tv_now, NULL);
      end_time_min = (tv_now.tv_sec - tv_start.tv_sec) * 1000000L + (tv_now.tv_usec - tv_start.tv_usec);
    }
  }

  // Protect writes to shared result variables
  pthread_mutex_lock(&results_mutex);
  if (time_for_max) *time_for_max = end_time_max;
  if (time_for_min) *time_for_min = end_time_min;
  data->max = local_max;
  data->min = local_min;
  pthread_mutex_unlock(&results_mutex);

  pthread_exit(NULL);
}


int main(int argc, char** argv)
{
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

  // ---------- ACTUAL MAIN CODE (creates N threads to find min and max in hugeTab)----------

  srand(time(NULL));

  int thread_number = atoi(bin_input_param);
  printf("Number of threads %d\n", thread_number);

  // Filling hugeTab with random values
  for (int i = 0; i < SIZE; i++) {
    hugeTab[i] = rand();
  }

  // Determining chunk size for each thread
  int chunk_size = SIZE / thread_number;
  int remainder = SIZE % thread_number;
  printf("Number of elements checked by each thread : %d\n", chunk_size);
  printf("Remaining elements to process : %d (handled by last thread if there are any)\n", remainder);

  int time_for_max = 0;
  int time_for_min = 0;

  // Table to store which the min / max values and their respective search times for each thread
  thread_data_t thread_data;
  thread_data.huge_table = hugeTab;
  thread_data.time_for_max = &time_for_max;
  thread_data.time_for_min = &time_for_min;

  pthread_t* thread_ids = malloc(thread_number * sizeof(pthread_t));

  int minimum_value, maximum_value;

  struct timeval program_start_tv, program_end_tv;
  gettimeofday(&program_start_tv, NULL);

  // For each chunk, create a thread searching for the lowest and highest values in its chunk of hugeTable
  for (int i = 0; i < thread_number; i++) {
    // Compute start and end indexes for the current thread
    int chunk_start = i * chunk_size;
    int chunk_end = (i + 1) * chunk_size;
    if (i == thread_number - 1) {
      chunk_end += remainder;
    }

    thread_data.chunk_start = chunk_start;
    thread_data.chunk_end = chunk_end;

    printf("\nThread %d processing indexes from %d to %d\n", i + 1, chunk_start, chunk_end - 1);

    // Create thread to iterate over its allocated chunk in given thread_function
    pthread_create(&thread_ids[i], NULL, thread_function, (void*) &thread_data);
    
    // Waiting for the current thread to finish
    pthread_join(thread_ids[i], NULL);

    // Lock while reading results written by the thread
    pthread_mutex_lock(&results_mutex);
    int t_max_time = thread_data.time_for_max ? *thread_data.time_for_max : 0;
    int t_min_time = thread_data.time_for_min ? *thread_data.time_for_min : 0;
    int t_max = thread_data.max;
    int t_min = thread_data.min;
    pthread_mutex_unlock(&results_mutex);

    printf("Time for max number search : %d microseconds\n", t_max_time);
    printf("Time for min number search : %d microseconds\n", t_min_time);
    printf("Thread %d found :\n\tmax : %d\n\tmin : %d\n", i + 1, t_max, t_min);

    // Updating min and max values found up to current handled chunk
    minimum_value = (i == 0) ? t_min : (t_min < minimum_value ? t_min : minimum_value);
    maximum_value = (i == 0) ? t_max : (t_max > maximum_value ? t_max : maximum_value);
  }

  printf("\nOverall minimum value found : %d\n", minimum_value);
  printf("Overall maximum value found : %d\n", maximum_value);

  gettimeofday(&program_end_tv, NULL);
  long total_microseconds = (program_end_tv.tv_sec - program_start_tv.tv_sec) * 1000000L
                + (program_end_tv.tv_usec - program_start_tv.tv_usec);
  printf("\nTotal execution time : %ld microseconds\n", total_microseconds);

  // Freeing allocated data
  free_if_needed(thread_ids);

  // Destroy the mutex used to protect results
  pthread_mutex_destroy(&results_mutex);

  return EXIT_SUCCESS;
}
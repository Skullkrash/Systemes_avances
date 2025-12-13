#include "../include/batch.h"

// Possible options to use when launching minishell
const char *minishell_options[] = {"-c", "--command", "--help", NULL};
Commands batch_command;
extern BackgroundProcess bg_processes;

int handle_arguments(int argc, const char *argv[]) {
    if (argc > MAX_ARGUMENTS)
    {
        fprintf(stderr, "Error: Too many arguments provided. Please use --help for usage details.\n");
        return EXIT_FAILURE;
    }

    const char *option = argv[1];

    int valid_option = 0;
    for (int i = 0; minishell_options[i] != NULL; i++) {
        if (strcmp(option, minishell_options[i]) == 0) valid_option = 1;
    }
    if (!valid_option) {
        fprintf(stderr, "Error: Unknown option %s. Please use --help for usage details.\n", option);
        return EXIT_FAILURE;
    }    

    if (strcmp(option, "--help") == 0) {
        printf("Usage: %s [options]\n", argv[0]);
        printf("Options:\n");
        printf("\t-c, --command <command>     Execute the specified command and exit\n");
        printf("\t--help                      Display this help message and exit\n");
        return EXIT_SUCCESS;
    }
    else if (strcmp(option, "-c") == 0 || strcmp(option, "--command") == 0) {
        if (argc > MAX_ARGUMENTS - 1) {
            split_line(strdup(argv[2]), &batch_command);

            execute_commands(&batch_command, &bg_processes);

            // add_to_history(batch_command);

            free_commands(&batch_command);

            return EXIT_SUCCESS;
        } else {
            fprintf(stderr, "Error: No command provided after option %s. Please use --help for usage details.\n", option);
            return EXIT_FAILURE;
        }
    }
    else {
        fprintf(stderr, "Error: Unknown option %s. Please use --help for usage details.\n", option);
        return EXIT_FAILURE;
    }
}
#include "../include/batch.h"

// Possible options to use when launching minishell
const char *minishell_options[] = {"-c", "--command", "--help", NULL};

Command batch_command = {NULL, 0, NULL, 0};

int handle_arguments(int argc, const char *argv[]) {
    if (argc > 3)
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
        if (argc > 2) {
            // batch_command.command = strdup(argv[2]);
            // batch_command.length = strlen(argv[2]);

            // parse_command(&current_command);
            // execute_command(&current_command);
            // add_to_history(current_command);

            // free_if_needed(current_command.command);
            // free_if_needed(current_command.args);

            // return batch_command;
            printf("Executing command: %s\n", argv[2]);
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
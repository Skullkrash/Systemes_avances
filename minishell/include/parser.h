#ifndef PARSER_H
#define PARSER_H

#include "typedef.h"
#include "internal_commands.h"

/**
 * @brief Detects if the current position in the input line matches any known operator.
 * @param p Pointer to the current position in the input line.
 * @param op_found Pointer to store the found operator string.
 * @return Length of the detected operator, or 0 if none found.
 */
int detect_operator(const char* p, const char** op_found);

/**
 * @brief Trims leading and trailing whitespace from a given line.
 * @param line Pointer to the input line.
 * @return Newly allocated string with trimmed whitespace.
 */
char* trim_whitespace(const char* line);

/**
 * @brief Splits the input line into commands and operators, populating the Commands struct.
 * @param line Pointer to the input line.
 * @param commands Pointer to the Commands struct to populate.
 */
void split_line(const char* line, Commands *commands);

/**
 * @brief Parses a command string into its arguments and populates the Command struct.
 * @param command Pointer to the Command struct to populate.
 */
void parse_command(Command* command);

#endif
/**
 * Logfind -- search for specific phrases in log files.
 * 1. Read the ~/.logfind file.
 * 2. Take each file, and search for the text.
 * 3. Return lines that contain the text, along with the line itself.
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

char *get_home_dir()
{
    char *home = getenv("HOME");
    if (home == NULL)
    {
        fprintf(stderr, "HOME environemnt not set.\n");
        return NULL;
    }
    return home;
}

char *get_full_path()
{
    // Get the home directory
    char *home_dir = get_home_dir();
    if (home_dir == NULL)
    {
        return NULL;
    }

    // Allocate memory for the ~/.logfind full path
    size_t home_len = strlen(home_dir);
    char *full_path = malloc(home_len + strlen(".logfind") + 2);
    if (full_path == NULL)
    {
        fprintf(stderr, "Could not allocate memory for path to ~/.logfind\n");
        return NULL;
    }

    // Construct the full path
    snprintf(full_path, home_len + strlen(".logfind") + 2, "%s/.logfind\n", home_dir);

    return full_path;
}

char **read_logfind(const char *filename, size_t *num_lines_ptr)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return NULL;
    }

    // Allocate memory for an array of pointers to strings
    char **lines = NULL;
    size_t num_lines = 0;
    size_t capacity = 0;

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, fp))
    {
        // Remove newline
        line[strcspn(line, "\n")] = '\0';

        // Re-allocate memory if necessary
        if (num_lines >= capacity)
        {
            capacity = capacity ? capacity * 2 : 16;
            lines = realloc(lines, capacity * sizeof(char *));
            if (lines == NULL)
            {
                fprintf(stderr, "Error re-allocating memory.\n");
                fclose(fp);
                return NULL;
            }
        }

        // Copy the line into the array
        lines[num_lines] = strdup(line);
        num_lines++;
    }

    fclose(fp);

    // Re-allocate memory to exact size needed
    lines = realloc(lines, num_lines * sizeof(char *));

    // Get the num_lines pointer
    *num_lines_ptr = num_lines;
    return lines;
}

int main(int argc, char *argv[])
{
    // Usage message if no argument is passed
    if (argc == 1)
    {
        fprintf(stderr, "Usage: %s -o <search_string>\n", argv[0]);
        fprintf(stderr, "Search for lines containing the string within log files defined in ~/.logfind\n");
        return 1;
    }

    char *search_string = NULL;
    // Find the -o flag, store the search_string
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-o") == 0)
        {
            if (i + 1 < argc)
            {
                search_string = argv[i + 1];
                break;
            }
            else
            {
                fprintf(stderr, "Error: no value passed after -o flag.\n");
                return 1;
            }
        }
        else
        {
            fprintf(stderr, "Error: -o flag not passed.\n");
            return 1;
        }
    }

    char *full_path = get_full_path();
    if (full_path == NULL)
    {
        return 1;
    }

    size_t num_lines;
    char **lines = read_logfind(full_path, &num_lines);
    if (lines == NULL)
    {
        free(full_path);
        return 1;
    }

    // Print matched lines within each file
    for (size_t i = 0; i < num_lines; i++)
    {
        FILE *fp = fopen(lines[i], "r");
        if (fp == NULL)
        {
            fprintf(stderr, "Error opening file: %s\n", lines[i]);
            continue; // Move to the next file
        }
        printf("[SEARCHING FILE]: %s for search string: %s\n", lines[i], search_string);

        int line_number = 1; // Initialize line number
        char line[MAX_LINE_LENGTH];
        while (fgets(line, MAX_LINE_LENGTH, fp))
        {
            if (strstr(line, search_string) != NULL)
            {
                printf("[MATCH FOUND]: %s, Line %d: \"%s\"\n", lines[i], line_number, line);
            }
            line_number++;
        }

        fclose(fp);
    }

    // Free allocated memory
    for (size_t i = 0; i < num_lines; i++)
    {
        free(lines[i]);
    }
    free(lines);
    free(full_path);

    return 0;
}

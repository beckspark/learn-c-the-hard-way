/**
 * Logfind -- search for specific phrases in log files.
 *  1. This tool takes any sequence of words and assumes I mean
 * “and” for them. So logfind zedshaw smart guy will find
 * all files that have zedshaw and smart and guy in them.
 * 2. It takes an optional argument of -o if the parameters are
 * meant to be or logic.
 * 3. It loads the list of allowed log files from ~/.logfind.
 * 4. The list of file names can be anything that the glob function
 * allows. Refer to man 3 glob to see how this works. I suggest
 * starting with just a flat list of exact files, and then add glob
 * functionality.
 * 5. You should output the matching lines as you scan, and try to
 * match them as fast as possible.
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
            printf("%s\n", argv[i]);
        }
    }

    char *full_path = get_full_path();
    if (full_path == NULL)
    {
        return 1;
    }

    size_t num_log_files;
    char **log_files = read_logfind(full_path, &num_log_files);
    if (log_files == NULL)
    {
        free(full_path);
        return 1;
    }

    // Search for log_files that contain the phrase
    for (size_t i = 0; i < num_log_files; i++)
    {
        FILE *fp = fopen(log_files[i], "r");
        if (fp == NULL)
        {
            fprintf(stderr, "Error opening file: %s\n", log_files[i]);
            continue; // Move to the next file
        }
        printf("Searching file %s for search string...\n", log_files[i]);

        int line_number = 1; // Initialize line number
        char line[MAX_LINE_LENGTH];
        while (fgets(line, MAX_LINE_LENGTH, fp))
        {
            int found = 1; // Start by assuming the line matches
            for (int j = 1; j < argc; j++)
            {
                if (strstr(line, argv[j]) == NULL)
                {
                    found = 0;
                    break;
                }
            }
            if (found)
            {
                printf("[MATCH FOUND]: %s, Line %d: %s\n", log_files[i], line_number, line);
            }
            line_number++;
        }

        fclose(fp);
    }

    // Free allocated memory
    for (size_t i = 0; i < num_log_files; i++)
    {
        free(log_files[i]);
    }
    free(log_files);
    free(full_path);

    return 0;
}

/**
 * Logfind -- search for specific phrases in log files.
 * 1. Read the ~/.logfind file.
 * 2. Take each file, and search for the text.
 * 3. Return lines that contain the text, along with the line itself.
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int read_logfind()
{
    FILE *fp;
    char buffer[100];

    // Get the home directory
    char *home_dir = get_home_dir();

    // Allocate memory for the ~/.logfind full path
    size_t home_len = strlen(home_dir);
    char *full_path = malloc(home_len + strlen(".logfind") + 2);
    if (full_path == NULL)
    {
        fprintf(stderr, "Could not allocate memory for path to ~/.logfind\n");
        return 1;
    }

    // Construct the full path
    snprintf(full_path, home_len + strlen(".logfind") + 2, "%s/.logfind\n", home_dir);

    // Open logfind
    fp = fopen(full_path, "r");

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        printf("%s", buffer);
    }

    fclose(fp);

    return 0;
}

int main()
{
    read_logfind();
}

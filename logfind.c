/**
 * Logfind -- search for specific phrases in log files.
 * 1. Read the ~/.logfind file.
 * 2. Take each file, and search for the text.
 * 3. Return lines that contain the text, along with the line itself.
 **/
#include <stdio.h>

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

    // Allocate memory for the full path
    size_t home_len = strlen(home_dir);
    char *full_path = malloc(home_len + strlen(".logfile") + 2);
    if (full_path == NULL)
    {
        fprintf(stderr, "Could not allocate memory for path to ~/.logfile\n");
        return 1;
    }

    sprintf(full_path, "%s/.logfile", home_dir);

    // Open logfind
    fp = fopen(home_dir, "r");

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

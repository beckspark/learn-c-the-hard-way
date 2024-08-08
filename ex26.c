/**
 * Logfind -- search for specific phrases in log files.
 * 1. Read the ~/.logfind file.
 * 2. Take each file, and search for the text.
 * 3. Return lines that contain the text, along with the line itself.
 **/
#include <stdio.h>

int main() {
    FILE *fp;
    char buffer[100];

    // Open the file
    fp = fopen("./log1.log", "r");

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    fclose(fp);

    return 0;
}

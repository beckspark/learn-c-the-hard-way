#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "dbg.h"

int print_a_message(const char *msg)
{
    printf("A STRING: %s", msg);

    return 0;
}

int uppercase(const char *msg)
{
    // Check string length before entering loop (resolve \0 termination problems)
    int i = 0, len = strlen(msg);

    for (i = 0; msg[i] < len; i++)
    {
        printf("%c", toupper(msg[i]));
    }

    printf("\n");

    return 0;
}

int lowercase(const char *msg)
{
    // Check string length before entering loop (resolve \0 termination problems)
    int i = 0, len = strlen(msg);

    for (i = 0; msg[i] < len; i++)
    {
        printf("%c", tolower(msg[i]));
    }
}

int fail_on_purpose(const char *msg)
{
    return 1;
}
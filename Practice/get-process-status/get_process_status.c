#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


int main(int argc, char *argv[])
{
    FILE *fp;
    ssize_t read;
    char *line = NULL;
    size_t len = 0;
    // if (argc < 2)
    // {
    //     printf("Provide a process in /proc\nHint: ./get_process_status <process-id>");
    //     return -1;
    // }

    fp = fopen("/proc/1543/status", "r");

    if (fp == NULL)
    {
        printf("Failed to open file, errno = %d\n",errno);
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len,fp)) != -1)
    {
        /* code */
        if (strstr(line, "State:") != NULL)
        {
            printf("%s",line);
        }
    }
    
    if (line)
    {
        free(line);
    }
    fclose(fp);
    return 0;
}
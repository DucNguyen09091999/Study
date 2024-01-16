#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


/*Get all process and its status*/
int list_process_status(const char *path)
{
    struct dirent *direntp = NULL;
    DIR *dirp = NULL;
    size_t path_len;
    int total_process = 0;
    /* Check input parameters. */
    if (!path)
    {
        exit(EXIT_FAILURE);
    }

    /* Open directory */
    dirp = opendir(path);
    if (dirp == NULL)
    {
        exit(EXIT_FAILURE);
    }

    /*Loop through all file in /proc folder*/
    while ((direntp = readdir(dirp)) != NULL)
    {
        char buf[10];
        char dir[20] = "/proc/";
        /*Ignore in case folder is '.', '..' and has first character is not a number*/
        if (!((strcmp(direntp->d_name, ".") == 0) ||
            (strcmp(direntp->d_name, "..") == 0) || !(atoi(strncpy(buf, direntp->d_name + 0,1)))))
        {
            total_process +=1;
            FILE *fp;
            ssize_t read;
            char *line = NULL;
            size_t len = 0;
            //printf("process id is %s\n",direntp->d_name);
            strcat(dir, direntp->d_name);
            strcat(dir, "/status");
            //printf("dir is %s\n",dir);

            fp = fopen(dir, "r");

            if (fp == NULL)
            {
                printf("Failed to open file, errno = %d\n",errno);
                exit(EXIT_FAILURE);
            }

            /*Loop through all line in file*/
            while ((read = getline(&line, &len,fp)) != -1)
            {
                /* In case the line content status */
                if (strstr(line, "State:") != NULL)
                {
                    if (strstr(line, "sleeping") != NULL)
                    {
                        printf("Process %s is sleeping\n",  direntp->d_name);
                    }
                    else if (strstr(line, "idle") != NULL)
                    {
                        printf("Process %s is idle\n",  direntp->d_name);
                    }
                    else if (strstr(line, "running") != NULL)
                    {
                        printf("Process %s is idle\n",  direntp->d_name);
                    }
                }
            }
    
            if (line)
            {
                free(line);
            }
            fclose(fp);
        }
    }
    /* Finalize resources. */
    (void)closedir(dirp);
    return total_process;
}

/* We are taking first argument as initial path name. */
int main(int argc, const char* argv[])
{
    int process_num;

    /*Get total number of process in /proc*/
    process_num = list_process_status("/proc");

    printf("Total process is %d\n", process_num);
    return 0;
}